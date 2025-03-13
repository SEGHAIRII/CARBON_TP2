#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <string>

class Problem {
public:
    int n; // number of jobs
    int m; // number of machines
    std::vector<std::vector<int>> processingTimes; // n x m matrix

    Problem();
    bool loadFromFile(const std::string &filename);
};

#endif // PROBLEM_H
