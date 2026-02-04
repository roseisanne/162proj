#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "file.h"

using namespace std;
//file input function
<vector<<int,int>, string> loadData(const string& filename) {
    vector<vector<int>> data;
    ifstream file(filename);

    if(!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    //reading
    string line;
    while(getline(file, line, ',')){
        if (line.empty()) continue;
    }
    //translate

    stringstream ss(line);

    while (ss >>) {

    }

    file.close();
    return data
}

main (){
     data = loadData("data.txt");
    for (const auto& row : data) {
        for (const auto& val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    return 0;
}