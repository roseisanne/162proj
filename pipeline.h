#pragma once

#include "CPU.h"
#include "tomasulo.h"
/*
3 stages of control

issue 
- if res station is available for needed functional unit
- read ready oprands (Vj/Vk)
- for operands that arent ready, rename the register to the res
station that will produce it (Qj/Qk)

execution 
- monitor the CDB for operand that is not readu
- when both operands are available, execute
- if more than one station per unit, only one station start execution
- do not start execution before prev banches have completed

write result
- write to CDB (and to registers) (maybe be struct hazard if only one
CDB bus)
- make res station (functional unit available)
*/

std::string rsType(const std::string& op);

void fetch(CPU& cpu);
void issue(CPU& cpu);

void broadcastResult(CPU& cpu, int rsIndex, float result);
void clearReservationStation(ReservationStation& rs);
void clearFunctionalUnit(FunctionalUnit& fu);

void execute(CPU& cpu);
void writeback(CPU& cpu);