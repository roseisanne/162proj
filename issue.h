#pragma once

#include "CPU.h"
#include "tomasulo.h"

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