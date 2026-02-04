#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;


struct function_unit {
    bool busy;
    int operation_type; //0: add, 1: sub, 2: mul, 3: div
    int destination;
    int source1;
    int source2;
    int address; //or immediate instructions
};

struct register_station {

};