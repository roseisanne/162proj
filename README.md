# 162proj
eval effect of diff arch parameters on CPU designs by simulating a modified(& simplified) version of PowerPC 604 and 620 archs. Assume 32-bit arch that exe. subset of RISC V ISA: fld, fsd, add, addi, fadd, fmul, fdiv, bne  
  
input file as command line input (prog.dat)  
Sim should read file, recognize instructions, recognize different fields of instructions, and simulate execution on arch  
implement functional+timing simulator
## The Arch
non-sepculative, single-issue, OOO CPU using Tomasulo's Alg (without ROB)  
a) fetch unit fetches NF=1 instruction every cycle  
b) no branch pred used. when bne is fetched/decoded, fetch unit stalls until branch completes exe. in branch unit and correct PC is det
c) decode unit can hold NI=1 instruction (bonus inc. size). Acts like simple pipeline latch btwn fetch and issue. If issue stage is stalled, decode stage holds instruction and signals the fetch unit to stall
d) NW=1 instruction can be issued every clk cycle from instruction queue to free reservation station

