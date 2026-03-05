#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <map>

#include "CPU.h"

//data memory <int, int>
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

//2. handel labels -> map loop to  current instr index, remove label from line and store clean instr
//3. convert to structured instr

int main ()
{
    std::vector<std::string> data = loadData("prog.dat");
    std::map<int, int> dataMem;
    std::vector<std::string> instrLines;
    std::map<std::string, int> labelMap;

    for (const auto& line: data)
    {
        std::stringstream ss(line);
        int addr{};
        int val{};
        char comma{};
        std::string loopLabel;

        //separate mem init and set of instructions
        if (ss >> addr >> comma >> val && comma == ',')
        {
            dataMem[addr] = val;
        }
        else 
        {
            if (line.find(':') != std::string::npos)
            {
                loopLabel = line.substr(0, line.find(':'));
                labelMap[loopLabel] = instrLines.size();
                instrLines.push_back(line.substr(line.find(':') + 1)); //store instr without label
            }
            else 
            {
                instrLines.push_back(line);
            }
        }
    }
    //checking
    std::cout << "Mem contents:\n";
    for (const auto& pair : dataMem)
    {
        std::cout << pair.first << " - " << pair.second << "\n";
    }

    std::cout << "Instr contents:\n";
    for (int i = 0; i < instrLines.size(); i++)
    {
        std::cout << instrLines.at(i) << "\n";
    }

    std::cout << "Labels:\n";
    for (const auto& p : labelMap)
    {
        std::cout << p.first << " -> " << p.second << "\n";
    }

    return 0;
}