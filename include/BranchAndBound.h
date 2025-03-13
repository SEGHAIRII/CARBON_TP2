#ifndef BRANCH_AND_BOUND_H
#define BRANCH_AND_BOUND_H

#include "Problem.h"
#include <vector>
#include <mutex>

struct Node {
    std::vector<int> schedule;      // Partial schedule (jobs scheduled so far)
    std::vector<bool> scheduled;    // Boolean vector marking scheduled jobs
    int depth;                      // Number of scheduled jobs
};

class BranchAndBound {
public:
    // 'parallelDepth' specifies up to which tree depth we spawn parallel tasks.
    BranchAndBound(const Problem &prob, int parallelDepth = 2);
    void run();
    int bestMakespan;
    std::vector<int> bestSchedule;
    
private:
    Problem problem;
    int parallelDepth;
    std::mutex bestMutex;
    void branch(const Node &node);
    int evaluateMakespan(const std::vector<int> &schedule);
};

#endif // BRANCH_AND_BOUND_H
