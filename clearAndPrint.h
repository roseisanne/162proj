#pragma once

#include "tomasulo.h"
#include "CPU.h"

void Instruction::print() const
{
    std::cout << "PC: " << pc << "\n" <<
    "Opcode: " << op << "\n" <<
    "rd: " << rd << "\n" << 
    "rs1: " << rs1 << "\n" <<
    "rs2: " << rs2 << "\n" <<
    "imm: " << imm << "\n" <<
    "targetPC: " << targetPC << "\n";
}

void ReservationStation::printStation()
{
    std::cout << "busy?: " << busy << "\n";
    std::cout << "RS op: " << op << "\n";
    std::cout << "dest: " << dest << "\n";
    std::cout << "Vj: " << Vj << " Vk: " << Vk << "\n";
    std::cout << "Qj: " << Qj << " Qk: " << Qk << "\n";
    std::cout << "A: " << A << "\n";
    std::cout << "type: " << type << "\n";
}

void clearReservationStation(ReservationStation& rs)
{
    rs.busy = false;
    rs.executing = false;
    rs.op = "";
    rs.Qj = -1;
    rs.Qk = -1;
    rs.Vj = 0;
    rs.Vk = 0;
    rs.A = 0;
    rs.dest = -1; 
}

void clearFunctionalUnit(FunctionalUnit& fu)
{
    fu.busy = false;
    fu.op = "";
    fu.rsTag = -1;
    fu.cyclesLeft = 0;
}