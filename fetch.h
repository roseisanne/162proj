#pragma once

#include "CPU.h"
#include "tomasulo.h"

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