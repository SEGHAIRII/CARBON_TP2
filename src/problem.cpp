#include "Problem.h"
#include <fstream>
#include <sstream>
#include <iostream>

Problem::Problem() : n(0), m(0) {}

bool Problem::loadFromFile(const std::string &filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file: " << filename << "\n";
        return false;
    }
    std::string line;
    // Read first line: number of jobs and machines
    if (std::getline(infile, line)) {
        std::istringstream iss(line);
        if (!(iss >> n >> m)) {
            std::cerr << "Invalid format in first line.\n";
            return false;
        }
    }
    processingTimes.resize(n, std::vector<int>(m, 0));
    for (int i = 0; i < n; i++) {
        if (std::getline(infile, line)) {
            std::istringstream iss(line);
            for (int j = 0; j < m; j++) {
                if (!(iss >> processingTimes[i][j])) {
                    std::cerr << "Invalid input format at job " << i << "\n";
                    return false;
                }
            }
        } else {
            std::cerr << "Not enough lines for jobs.\n";
            return false;
        }
    }
    return true;
}
