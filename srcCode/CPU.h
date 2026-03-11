#pragma once

#include "tomasulo.h"

// data memory
// instr list
// label -> pc map

struct Instruction 
{
    std::string op;
    //int reg
    int rd{-1};
    int rs1{-1};
    int rs2{-1};
    //immed
    int imm{0};
    //branch traget
    int targetPC{-1};

    //for debugging purposes
    int pc{-1};
    void print()
    {
    std::cout << "PC: " << pc << "\n" <<
    "Opcode: " << op << "\n" <<
    "rd: " << rd << "\n" << 
    "rs1: " << rs1 << "\n" <<
    "rs2: " << rs2 << "\n" <<
    "imm: " << imm << "\n" <<
    "targetPC: " << targetPC << "\n";
    }
};

//to know that an instr is still being used
struct Decode
{
    bool busy{false};
    Instruction instr;
};

struct CPU
{
    int PC{0};
    int cycles{0};

    std::vector<Instruction> instrMem;
    std::map<int, double> dataMem;

    std::vector<ReservationStation> RS;

    std::vector<FunctionalUnit> FUs;

    std::vector<float> registers;
    std::vector<RegisterStatus> regStatus;

    CDB cdb;
    Decode decode;

    bool branchPending{false};

    //stall counters
    int fetchStalls{0};
    int issueStalls{0};
    int executeStalls{0};

    CPU() //constructor
    {
        RS.reserve(14); //total number of stations
        for(int i=0;i<4;++i) { RS.push_back(ReservationStation("int")); }
        for(int i=0;i<3;++i) { RS.push_back(ReservationStation("ldst")); }
        for(int i=0;i<3;++i) { RS.push_back(ReservationStation("fpadd")); }
        for(int i=0;i<2;++i) { RS.push_back(ReservationStation("fpmul")); }
        RS.push_back(ReservationStation("fpdiv"));
        RS.push_back(ReservationStation("bu"));

        //
        FUs.push_back({false, "int"});
        FUs.push_back({false, "ldst"});
        FUs.push_back({false, "fpadd"});
        FUs.push_back({false, "fpmul"});
        FUs.push_back({false, "fpdiv"});
        FUs.push_back({false, "bu"});
        
        registers.resize(64, 0); //R0-R31 and F0-F31
        regStatus.resize(64);
    }
};