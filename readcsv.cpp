#include <fstream>
#include <string>
#include <sstream>
#include <vector>

const char* readcsv(const char* f, int r, int c) {
    static std::string res;
    std::ifstream file(f);
    std::string line;
    std::getline(file, line);
    for (int i = 0; i <= r && std::getline(file, line); i++) {
        if(line.find("NOT COMPLETED") != std::string::npos) {
            i--;
            continue;
        }
        if(i == r) {
            std::vector<std::string> cols;
            std::string cell;
            for (std::stringstream ss(line); std::getline(ss, cell, ','); cols.push_back(cell));
            if (c >= 0 && c < cols.size()) { res = cols[c]; return res.c_str(); }
        }
    }
    res = "ERROR";
    return res.c_str();
}