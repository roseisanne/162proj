#pragma once

#include "CPU.h"

//so that the correct op instr finds the first available res station for that op
std::string rsType(const std::string& op);

void fetch(CPU& cpu);
void issue(CPU& cpu);

//when result is broadcasted on CDB, any res Station waiting for that
//tag must capture that val immediately in same cycle
void forwarding(CPU& cpu, int rsIndex, float result);

void broadcastResult(CPU& cpu, int rsIndex, float result);
//resets
void clearReservationStation(ReservationStation& rs);
void clearFunctionalUnit(FunctionalUnit& fu);

int cyclesForOperation(ReservationStation& rs);
void execute(CPU& cpu);

//to produce result of ALU instructions
float ALUforWrite(CPU& cpu, ReservationStation& rs);
void writeback(CPU& cpu);