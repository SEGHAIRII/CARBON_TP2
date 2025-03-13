#ifndef PALMERS_HEURISTIC_H
#define PALMERS_HEURISTIC_H

#include "Heuristic.h"

class PalmersHeuristic : public Heuristic {
public:
    explicit PalmersHeuristic(const Problem &prob) : problem(prob) {}
    std::vector<int> solve() override;

private:
    const Problem &problem;
};

#endif // PALMERS_HEURISTIC_H
