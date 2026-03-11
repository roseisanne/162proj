#include "pipeline.h"

//makes sure that the correct operations will be set into the correct
//res station and will do correct action
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

//Update registers waiting for this RS
void forwarding(CPU& cpu, int rsIndex, float result)
{
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

//will show results 
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

    forwarding(cpu, rsIndex, result);
}

void clearReservationStation(ReservationStation& rs)
{
    rs.busy = false;
    rs.executing = false;
    rs.op = "";
    rs.Qj = -1;
    rs.Qk = -1;
    rs.Vj = 0;
    rs.Vk = 0;
    rs.A = 0;
    rs.dest = -1; 
}

void clearFunctionalUnit(FunctionalUnit& fu)
{
    fu.busy = false;
    fu.op = "";
    fu.rsTag = -1;
    fu.cyclesLeft = 0;
}

int cyclesForOperation(ReservationStation& rs)
{
    int num{};
    if (rs.type == "int") { num = 1; }
    else if (rs.type == "ldst") { num = 2; }
    else if (rs.type == "fpadd") { num = 3; }
    else if (rs.type == "fpmul") { num = 4; }
    else if (rs.type == "fpdiv") { num = 6; }
    else if (rs.type == "bu") { num = 1; }
    //so that writeback fetches correct result in same cycle
    return num - 1;
}

float ALUforWrite(CPU& cpu, ReservationStation& rs)
{
    float result = 0;
    if (rs.op == "addi") { result = rs.Vj + rs.A; }
    else if (rs.op == "add") { result = rs.Vj + rs.Vk; }
    else if (rs.op == "fld") { result = cpu.dataMem[rs.Vj + rs.A]; }
    else if (rs.op == "fadd") { result = rs.Vj + rs.Vk; }
    else if (rs.op == "fmul") { result = rs.Vj * rs.Vk; }
    else if (rs.op == "fdiv") { result = rs.Vj / rs.Vk; }
    return result;
}