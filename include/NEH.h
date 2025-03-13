#ifndef NEH_H
#define NEH_H

#include "Heuristic.h"

class NEH : public Heuristic {
public:
    explicit NEH(const Problem &prob) : problem(prob) {}
    std::vector<int> solve() override;

private:
    const Problem &problem;
    int bestInsertionPosition(const std::vector<int> &partialSchedule, int job);
};

#endif // NEH_H
