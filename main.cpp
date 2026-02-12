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

using namespace std;
//data memory <int, int>
//instr memory <string>

//file input function
vector<string> loadData(const string& filename) {
    vector<string> lines;
    ifstream file(filename);

    if(!file.is_open()) {
        cerr << "Error opening file: " << filename << "\n";
        return lines;
    }
    //reading
    string line;
    while(getline(file, line)){
        lines.push_back(line);
    }

    file.close();
    return lines;
}

main (){
    vector<string> data = loadData("prog.dat");
    for (const auto& line: data){
        cout << line << "\n";
    }

    return 0;
}