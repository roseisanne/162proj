#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>

/*
intruction status:
- issued, read operands and started execution, completed execution
or wrote result

status for Functional unit: (assuming non-pipelined units)
-busy/not
-operation(if unit can perform more than one)
-destination reg, Fi
-source registers(containing source operands) - Fj and Fk
- unit producing source operands - Qj and Qk
- flags to indicated source operands are ready - Rj and Rk

Register result status:
- indicated functional unit that contains and instr which will write
into each register (if any)
*/

/*
Tomasulo -
- each rev station has id and is used by one instr during lifetime of
this instr
unit       latency(cycles for op)            res stations       instrs
int            1(int and logic op)              4               add, addi
ld/str         2                                3                fld, fsd
fpadd          3                                3               fadd
fpmult         4                                2               fmul
fpdiv          6                                1               fdiv
BU             1(condition and target eval)     1               bne
*/

struct ReservationStation
{
    bool busy{false};
    std::string op{""};
    float Vj{0};
    float Vk{0};
    int Qj{-1};
    int Qk{-1};
    int A{0}; //ld/str addresses

    //destination register
    int dest{-1};
    std::string type{""};
    bool executing{false};

    ReservationStation(std::string t) : type(t) {}

    void printStation();
};

struct RegisterStatus
{
    bool ready{true}; 

    //of RS station
    int tag{-1};
};

struct FunctionalUnit
{
    bool busy{false};

    std::string type{""};
    std::string op{""};
    
    //tag of the reservation station currently using this unit
    int rsTag{-1};
    //cycles left for current operation
    int cyclesLeft{0};
};

struct CDB
{
    bool busy{false};
    //tag of the reservation station broadcasting result
    int tag{-1};
    //value being broadcast
    float value{0};
};

//this is so that the the type of intructions correctly go into the correct res station
std::string rsType(const std::string& op)
{
    if(op == "add" || op == "addi") { return "int"; }
    if(op == "fld" || op == "fsd") { return "ldst"; }
    if(op == "fadd") { return "fpadd"; }
    if(op == "fmul") { return "fpmul"; }
    if(op == "fdiv") { return "fpdiv"; }
    if(op == "bne") { return "bu"; }
    return "";
};