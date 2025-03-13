#include "CDS.h"
#include <algorithm>
#include <limits>

// Solve the CDS heuristic
std::vector<int> CDS::solve() {
    int n = problem.n;
    int m = problem.m;
    
    std::vector<std::vector<int>> bestSchedule;
    int bestMakespan = std::numeric_limits<int>::max();
    std::vector<int> bestOrder;

    // Generate m-1 two-machine subproblems
    for (int i = 1; i < m; i++) {
        std::vector<std::vector<int>> twoMachineProcessing(n, std::vector<int>(2, 0));

        for (int j = 0; j < n; j++) {
            twoMachineProcessing[j][0] = std::accumulate(problem.processingTimes[j].begin(), problem.processingTimes[j].begin() + i, 0);
            twoMachineProcessing[j][1] = std::accumulate(problem.processingTimes[j].begin() + i, problem.processingTimes[j].end(), 0);
        }

        std::vector<int> schedule = solveTwoMachineProblem(twoMachineProcessing);
        int makespan = evaluateMakespan(problem, schedule);

        if (makespan < bestMakespan) {
            bestMakespan = makespan;
            bestOrder = schedule;
        }
    }

    return bestOrder;
}

// Solve a two-machine flow shop scheduling problem using Johnson’s Rule
std::vector<int> CDS::solveTwoMachineProblem(const std::vector<std::vector<int>> &processingTimes) {
    int n = processingTimes.size();
    std::vector<int> jobs(n);
    std::iota(jobs.begin(), jobs.end(), 0);

    // Apply Johnson’s Rule
    std::stable_sort(jobs.begin(), jobs.end(), [&processingTimes](int a, int b) {
        int minA = std::min(processingTimes[a][0], processingTimes[a][1]);
        int minB = std::min(processingTimes[b][0], processingTimes[b][1]);

        if (minA == minB) {
            return processingTimes[a][0] < processingTimes[b][0];
        }
        return minA < minB;
    });

    return jobs;
}
