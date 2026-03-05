#pragma once
#include "cpu.h"

#include <vector>
#include <map>
#include <sstream>

std::vector<Instruction> parseInstructions(
    const std::vector<std::string>& instrLines, 
    std::map<std::string, int>& labelMap);