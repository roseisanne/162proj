#pragma once

#include "clearAndPrint.h"
#include "tomasulo.h"
#include "CPU.h"

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
        if (taken) { cpu.PC = rs.A; } //branch taken
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