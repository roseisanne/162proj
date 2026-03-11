#pragma once

#include "pipeline.h"

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

        //handle instrs only reading
        if (rs.op == "bne")
        {
            bool taken = rs.Vj != rs.Vk;
            if (taken) { cpu.PC = rs.A; } //branch taken
            //else PC go to next sequential instruction
            cpu.decode.busy = false; // branch resolved, fetch can continue
            cpu.branchPending = false;
        }
        else if (rs.op == "fsd") { cpu.dataMem[rs.Vj + rs.A] = rs.Vk; }
        else //else, instr that only write
        {
            broadcastResult(cpu, rsIndex, ALUforWrite(cpu, rs));
            cdbUseCurrCycle = true;
        }

        clearFunctionalUnit(fu);
        clearReservationStation(rs);
    }

    cpu.cdb.busy = false;
}