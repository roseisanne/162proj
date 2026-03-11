#pragma once
#include "cpu.h"

#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <algorithm>

std::vector<Instruction> parseInstructions(
    const std::vector<std::string>& instrLines, 
    const std::map<std::string, int>& labelMap)
{
    std::vector<Instruction> instrMem;

    //go through each line of instruction, parse it into
    //structured instructions and store in instrMem
    for(int pc = 0; pc < instrLines.size(); ++pc)
    {
        std::string line = instrLines[pc];
        if(line.empty()) { continue; }
        line.erase(std::remove(line.begin(), line.end(), ','), line.end());

        int colon = line.find(':');
        if(colon != std::string::npos)
        {
            line = line.substr(colon + 1);
        }

        std::stringstream ss(line);
        std::string op;
        ss >> op;

        Instruction instr;
        instr.op = op;
        instr.pc = pc;

        if(op == "addi")
        {
            std::string rd, rs1;
            int imm;
            ss >> rd >> rs1 >> imm;
            instr.rd = std::stoi(rd.substr(1)); //just stores the number in instr
            instr.rs1 = std::stoi(rs1.substr(1));
            instr.imm = imm;
        }
        else if(op == "add" || op == "fadd" || op == "fmul" || op == "fdiv")
        {
            std::string rd, rs1, rs2;
            ss >> rd >> rs1 >> rs2;
            instr.rd = std::stoi(rd.substr(1)) + 32;
            instr.rs1 = std::stoi(rs1.substr(1)) + 32;
            instr.rs2 = std::stoi(rs2.substr(1)) + 32;
        }
        else if(op == "fld") 
        {
            std::string frd, offsetR;
            ss >> frd >> offsetR;
            instr.rd = std::stoi(frd.substr(1)) + 32;
            size_t leftParenth = offsetR.find('(');
            size_t rightParenth = offsetR.find(')');

            instr.imm = std::stoi(offsetR.substr(0, leftParenth));
            std::string baseReg = offsetR.substr(leftParenth  + 1, 
                                rightParenth - leftParenth - 1);
            instr.rs1 = std::stoi(baseReg.substr(1));
        }
        else if (op == "fsd")
        {
            std::string frs, offsetR;
            ss >> frs >> offsetR;
            instr.rs2 = std::stoi(frs.substr(1)) + 32;
            size_t leftParenth = offsetR.find('(');
            size_t rightParenth = offsetR.find(')');

            instr.imm = std::stoi(offsetR.substr(0, leftParenth));
            std::string baseReg = offsetR.substr(leftParenth  + 1, 
                                rightParenth - leftParenth - 1);
            instr.rs1 = std::stoi(baseReg.substr(1));
        }
        else if (op == "bne")
        {
            std::string rs1, rs2, label;
            ss >> rs1 >> rs2 >> label;
            instr.rs1 = std::stoi(rs1.substr(1));
            instr.rs2 = std::stoi(rs2.substr(1));
            instr.targetPC = labelMap.at(label);
        }

        instrMem.push_back(instr);
    }
    return instrMem; 
}