#ifndef CDS_H
#define CDS_H

#include "Heuristic.h"
#include "CDS.h"
#include <algorithm>
#include <numeric>  // ✅ Required for std::accumulate and std::iota
#include <limits>


class CDS : public Heuristic {
public:
    explicit CDS(const Problem &prob) : problem(prob) {}
    std::vector<int> solve() override;

private:
    const Problem &problem;
    std::vector<int> solveTwoMachineProblem(const std::vector<std::vector<int>> &processingTimes);
};

#endif // CDS_H
