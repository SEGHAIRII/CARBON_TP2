#ifndef MNEH_H
#define MNEH_H

#include "Heuristic.h"
#include "MNEH.h"
#include <algorithm>
#include <numeric>  // ✅ Required for std::accumulate
#include <limits>


class MNEH : public Heuristic {
public:
    explicit MNEH(const Problem &prob) : problem(prob) {}
    std::vector<int> solve() override;

private:
    const Problem &problem;
    int bestInsertionPosition(const std::vector<int> &partialSchedule, int job);
};

#endif // MNEH_H
