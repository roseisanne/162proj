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

struct memory {
    vector<int> addresses;
    vector<int> values;
    int size;

    memory(){
        size = 0;
    }

    memory(int sz) {
        size = sz;
        addresses.resize(sz);
        values.resize(sz);
    }
};