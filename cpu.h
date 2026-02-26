#pragma once
//fetch unit fetched NF=1 instr every cycle

//when branch is fetched/decoded, fetch unit stalls until branch instr
//completes execution in branch unit and correct next PC determined

//decode unit can only hold NI=1 instr - acts as pipeline latch between
//fetch and issue, issue stage stalled, decode stage holds instr
//and signals detch unit to stall

//up to NW=1 instr issued every clk cycle from the instruction queue to
//free reservation station.

//single common stat bus (nb=1) used to connect functional units to
//register file and reservation stations
// - instrs commit as soon as they finish execution and write result to the CDB
// - if mult functional units finish execution at the same cycle, unit
//   with the oldest instr gets priority to broadcast result, other
//   units must stall and wait till next cycle to retry

//implement register renaming using Reservation Station IDs as tags
// - use register status table: tracks whether register's value is
//   currently valid in register file or if pending result from specific 
//   Reservation Station ID

//implement forwarding vis CDB, when result broadcast on CDB, any
//reservation station waiting for that specific tag must capture value
//immediatly in same cycle

// assume that the entire program fits in the instruction cache
// (i.e., it always takes one cycle to read a cache line). Also, 
// the data cache is perfect (hit rate =100%) and single-ported. Reading 
// or writing a word always takes one cycle. There are no cache
// misses to simulate. You may assume a logical separation between 
// Instruction Memory and Data Memory. Use the addresses specified in the input file for data memory. Instructions are stored in
// a separate list starting at Index 0. The Program Counter (PC) 
// simply tracks the index of the instruction in 
// this list (PC, PC+1, PC+2...)

//keep statistic about
//total executiion cycles
//stall events



/* data memory
instr list
label -> pc map*/

struct Instruction 
{
    std::string op;
    //int reg
    int rd, rs1, rs2;
    //FP registers
    int frd, frs1, frs2;
    //immed
    int imm;
    //branch traget
    int targetPC;
};