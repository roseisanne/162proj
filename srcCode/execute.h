#pragma once

#include "pipeline.h"

void execute(CPU& cpu)
{
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

        fu->cyclesLeft = cyclesForOperation(rs);
        
        rs.executing = true;
    }

    //decrement cycles on FUs 
    for (auto& i : cpu.FUs)
    {
        if (i.busy && i.cyclesLeft > 0) { i.cyclesLeft--; }
    }
}