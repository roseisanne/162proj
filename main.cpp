#include <cmath>
#include <fstream>
#include <stdlib.h>

#include "parse.h"
#include "fetch.h"
#include "issue.h"
#include "execute.h"
#include "writeback.h"

//data memory <int, double (for the .0 stuff)>
//instr memory <string>

//file input function
std::vector<std::string> loadData(const std::string& filename) 
{
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if(!file.is_open()) 
    {
        std::cerr << "Error opening file: " << filename << "\n";
        return lines;
    }
    //reading
    std::string line;
    while(std::getline(file, line))
    {
        lines.push_back(line);
    }

    file.close();
    return lines;
}

int main ()
{
    std::vector<std::string> data{loadData("prog.dat")};
    std::map<int, double> dataMem;
    std::vector<std::string> instrLines;
    std::map<std::string, int> labelMap; //get loop lable

    for (const auto& line: data)
    {
        std::stringstream ss(line);
        int addr{};
        int val{};
        char comma{','};
        std::string loopLabel;

        //separate init data mem and set of instructions
        if (ss >> addr >> comma >> val)
        {
            dataMem[addr] = val;
        }
        else 
        {
            if (line.find(':') != std::string::npos)
            {
                loopLabel = line.substr(0, line.find(':'));
                labelMap[loopLabel] = instrLines.size();
                //store instr without label
                instrLines.push_back(line.substr(line.find(':') + 2));
            }
            else 
            {
                instrLines.push_back(line);
            }
        }
    }

    CPU cpu;

    cpu.dataMem = dataMem;
    cpu.instrMem = parseInstructions(instrLines, labelMap);

    bool programComplete = false;

    //start program
    //do this for each instruction in code
    while (!programComplete)
    {
        cpu.cycles++;
        
        // (cpu.cycles > 500) 
        // { 
        // std::cout << "INFINITE LOOP\n"; 
        // break; 
        // }

        writeback(cpu);
        execute(cpu);
        issue(cpu);
        fetch(cpu);

        //checks each cycle if everything is clear at the end so that it
        //knows when to stop
        bool rsBusy = false;
        for (auto& rs : cpu.RS)
        {
            if (rs.busy) { rsBusy = true; }
        }
        bool fuBusy = false;
        for (auto& fu : cpu.FUs) 
        {
            if (fu.busy) { fuBusy = true; }
        }

        if (cpu.PC >= cpu.instrMem.size() 
            && !cpu.decode.busy && !rsBusy && !fuBusy)
        {
            programComplete = true;
        }
    }
    std::cout << "----------------------\n";

    //prgram results
    std::cout << "Program finished in " << cpu.cycles << " cycles.\n";
    std::cout << "total Stall cycles: " << 
    cpu.fetchStalls + cpu.issueStalls + cpu.executeStalls << " cycles.\n";

    //what is in data mem after program
    std::cout << "\nData Memory:\n";
    for (const auto& pair : cpu.dataMem)
    {
        std::cout << "  [" << pair.first << "] = " << pair.second << "\n";
    }
    std::cout << "----------------------\n";

    //what are in the registers (in this case)
    std::cout << "Registers:\n";
    for (int i = 0; i < 6; i++) //r
    {
        std::cout << "  R" << i << " = " << cpu.registers[i] << "\n";
    }
    for (int i = 32; i < 38; i++) //f
    {
       std::cout << "  F" << (i-32) << " = " << cpu.registers[i] << "\n";
    }

    return 0;
}

//checking contents printing
/*
    std::cout << "Mem contents:\n";
    for (const auto& pair : dataMem)
    {
        std::cout << pair.first << " - " << pair.second << "\n";
    }

    std::cout << "Labels:\n";
    for (const auto& p : labelMap)
    {
        std::cout << p.first << " -> " << p.second << "\n";
    }

    std::cout << "Parsed Instr:\n";
    for (const auto& instr : cpu.instrMem)
    {
        instr.print();
        std::cout << "------------------------\n";
    }

    std::cout << "Cycle " << cpu.cycles << " PC=" << cpu.PC 
          << " branchPending=" << cpu.branchPending 
          << " decode.busy=" << cpu.decode.busy << "\n"
          << " R1=" << cpu.registers[1] 
          << " R1ready=" << cpu.regStatus[1].ready << "\n";
    */