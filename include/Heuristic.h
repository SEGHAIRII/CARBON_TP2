#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Problem.h"
#include <vector>

class Heuristic {
public:
    virtual ~Heuristic() = default;
    virtual std::vector<int> solve() = 0;
    
    // Declare evaluateMakespan function
    int evaluateMakespan(const Problem &problem, const std::vector<int> &schedule);
};

#endif // HEURISTIC_H
