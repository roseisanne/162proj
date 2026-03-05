#include "parse.h"

std::vector<Instruction> parseInstructions(
    const std::vector<std::string>& instrLines,
    const std::map<std::string,int>& labelMap)
{
    std::vector<Instruction> instrMem;

    //go through each line of instruction, parse it into
    //structured instructions and store in instrMem
    for(int pc = 0; pc < instrLines.size(); ++pc)
    {
        std::string line = instrLines[pc];
        if(line.empty()) continue;

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
            char comma;
            ss >> rd >> comma >> rs1 >> comma >> imm;
            instr.rd = rd[1] - '0';
            instr.rs1 = rs1[1] - '0';
            instr.imm = imm;
        }
        else if(op == "add" || op == "fadd" || op == "fmul" || op == "fdiv")
        {
            std::string rd, rs1, rs2;
            char comma;
            ss >> rd >> comma >> rs1 >> comma >> rs2;
            instr.rd = rd[1] - '0';
            instr.rs1 = rs1[1] - '0';
            instr.rs2 = rs2[1] - '0';
        }
        else if(op == "fld") 
        {
            std::string frd, offsetR;
            ss >> frd >> offsetR;
            instr.rd = frd[1] - '0';
            size_t leftParen = offsetR.find('(');
            size_t rightParen = offsetR.find(')');
            instr.imm = std::stoi(offsetR.substr(0, leftParen));
            instr.rs1 = offsetR.substr(leftParen + 1, rightParen - leftParen - 1)[1] - '0';
        }
        else if (op == "fsd")
        {
            std::string frs, offsetR;
            ss >> frs >> offsetR;
            instr.rs2 = frs[1] - '0';
            size_t leftParen = offsetR.find('(');
            size_t rightParen = offsetR.find(')');
            instr.imm = std::stoi(offsetR.substr(0, leftParen));
            instr.rs1 = offsetR.substr(leftParen + 1, rightParen - leftParen - 1)[1] - '0';
        }
        else if (op == "beq")
        {
            std::string rs1, rs2, label;
            char comma;
            ss >> rs1 >> comma >> rs2 >> comma >> label;
            instr.rs1 = rs1[1] - '0';
            instr.rs2 = rs2[1] - '0';
            instr.targetPC = labelMap.at(label);
        }
        else if (op == "bne")
        {
            std::string rs1, rs2, label;
            char comma;
            ss >> rs1 >> comma >> rs2 >> comma >> label;
            instr.rs1 = rs1[1] - '0';
            instr.rs2 = rs2[1] - '0';
            instr.targetPC = labelMap.at(label); //instr index
        }

        instrMem.push_back(instr);
    }
    return instrMem; 
}