#include "pipeline.h"

std::string rsType(const std::string& op)
{
    if(op == "add" || op == "addi") { return "int"; }
    if(op == "fld" || op == "fsd") { return "ldst"; }
    if(op == "fadd") { return "fpadd"; }
    if(op == "fmul") { return "fpmul"; }
    if(op == "fdiv") { return "fpdiv"; }
    if(op == "bne") { return "bu"; }
    return "";
};

void fetch(CPU& cpu)
{
    //stall if decode is busy
    if(cpu.decode.busy) 
    { 
        cpu.fetchStalls++;
        return; 
    }

    //stop if program is finished
    if(cpu.PC >= cpu.instrMem.size()) { return; }

    //fetch instr
    cpu.decode.instr = cpu.instrMem[cpu.PC];
    cpu.decode.busy = true;
    cpu.PC++;
};

void issue(CPU& cpu)
{
    if (!cpu.decode.busy) { return; } 
    Instruction instr = cpu.decode.instr;

    //Find open RS
    int freeRS = -1;
    for (int i = 0; i < cpu.RS.size(); i++)
    {
        if(!cpu.RS[i].busy && cpu.RS[i].type == rsType(instr.op))
        {
            freeRS = i;
            break;
        }
    }
    //stall if no RS avail for that op
    if(freeRS == -1 || cpu.branchPending) 
    { 
        cpu.issueStalls++;
        return; 
    }

    //Filling the station
    ReservationStation& rs = cpu.RS[freeRS];
    rs.busy = true;
    rs.op = instr.op;
    rs.type = rsType(instr.op);
    rs.dest = instr.rd; //-1 if store

    //souce operands
    if (instr.rs1 != -1){
        if (cpu.regStatus[instr.rs1].ready)
        {
            rs.Vj = cpu.registers[instr.rs1];
        }
        else 
        {
            rs.Qj = cpu.regStatus[instr.rs1].tag;
        }
    }
    if (instr.rs2 != -1)
    {
        if (cpu.regStatus[instr.rs2].ready)
        {
            rs.Vk = cpu.registers[instr.rs2];
        }
        else
        {
            rs.Qk = cpu.regStatus[instr.rs2].tag;
        }
    }

    //imm offset
    if(instr.op == "fld" || instr.op == "fsd" || instr.op == "addi")
    {
        rs.A = instr.imm;
    }

    //register renaming
    if (instr.rd != -1)
    {
        cpu.regStatus[instr.rd].ready = false;
        cpu.regStatus[instr.rd].tag = freeRS;
    }

    if(instr.op == "bne")
    {
        rs.A = instr.targetPC;
        cpu.branchPending = true;
    }
    cpu.decode.busy = false;
}

void broadcastResult(CPU& cpu, int rsIndex, float result)
{
    cpu.cdb.busy = true;
    cpu.cdb.tag = rsIndex;
    cpu.cdb.value = result;

    for (auto& otherRS : cpu.RS)
    {
        if (otherRS.Qj == rsIndex)
        {
            otherRS.Vj = result;
            otherRS.Qj = -1;
        }
        if (otherRS.Qk == rsIndex)
        {
            otherRS.Vk = result;
            otherRS.Qk = -1;
        }
    }

    //Update registers waiting for this RS
    for (int i = 0; i < cpu.regStatus.size(); i++)
    {
        if (!cpu.regStatus[i].ready && cpu.regStatus[i].tag == rsIndex)
        {
            cpu.registers[i] = result;
            cpu.regStatus[i].tag = -1;
            cpu.regStatus[i].ready = true;
        }
    }
}

void execute(CPU& cpu)
{
    //decrement cycles on FUs 
    for (auto& f : cpu.FUs)
    {
        if (f.busy && f.cyclesLeft > 0) { f.cyclesLeft--; }
    }

    //starting new instrs
    for (auto& rs: cpu.RS)
    {
        //is RS not busy
        if (!rs.busy || rs.executing) { continue; }
        //check operands ready
        if (rs.Qj != -1 || rs.Qk != -1) { continue; }

        //find freeFU of matching type
        FunctionalUnit* fu = nullptr;
        for (auto& f : cpu.FUs) 
        {
            if (!f.busy && f.type == rs.type)
            {
                fu = &f;
                break;
            }
        }
        if (!fu) //stall if no FU available
        { 
            cpu.executeStalls++;
            continue; 
        }

        //start execution
        fu->busy = true;
        fu->op = rs.op;
        fu->rsTag = &rs - &cpu.RS[0]; //store index as tag

        //-1 since issue counts a cycle 1
        if (rs.type == "int") { fu->cyclesLeft = 0; }
        else if (rs.type == "ldst") { fu->cyclesLeft = 1; }
        else if (rs.type == "fpadd") { fu->cyclesLeft = 2; }
        else if (rs.type == "fpmul") { fu->cyclesLeft = 3; }
        else if (rs.type == "fpdiv") { fu->cyclesLeft = 5; }
        else if (rs.type == "bu") { fu->cyclesLeft = 0; }
        rs.executing = true;
    }
}

void writeback(CPU& cpu)
{
    bool cdbUseCurrCycle = false;
    for (auto& fu : cpu.FUs)
    {
        if (!fu.busy || fu.cyclesLeft > 0) { continue ;}
        //1 broadcast per cycle
        if (cdbUseCurrCycle) { break; }

        int rsIndex = fu.rsTag;
        ReservationStation& rs = cpu.RS[rsIndex];

        //calc result
        float result = 0;
        if (rs.op == "addi") { result = rs.Vj + rs.A; }
        else if (rs.op == "add") { result = rs.Vj + rs.Vk; }
        else if (rs.op == "fld") { result = cpu.dataMem[rs.Vj + rs.A]; }
        else if (rs.op == "fsd") { cpu.dataMem[rs.Vj + rs.A] = rs.Vk; }
        else if (rs.op == "fadd") { result = rs.Vj + rs.Vk; }
        else if (rs.op == "fmul") { result = rs.Vj * rs.Vk; }
        else if (rs.op == "fdiv") { result = rs.Vj / rs.Vk; }
        if (rs.op == "bne") //handle branch
        {
        bool taken = false;
        if (rs.op == "bne") { taken = (rs.Vj != rs.Vk); }
        if (taken) cpu.PC = rs.A; //branch taken

        //else PC is already at next sequential instruction
        cpu.decode.busy = false; // branch resolved, fetch can continue
        cpu.branchPending = false;
        }

        //broadcast on CDB
        if (rs.op != "fsd" && rs.op != "bne")
        {
            broadcastResult(cpu, rsIndex, result);
            cdbUseCurrCycle = true;
        }

        clearFunctionalUnit(fu);
        clearReservationStation(rs);
    }

    cpu.cdb.busy = false;
}