#pragma once

#include "clearAndPrint.h"
#include "tomasulo.h"
#include "CPU.h"

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