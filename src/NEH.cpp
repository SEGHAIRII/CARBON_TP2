#include "NEH.h"
#include <algorithm>
#include <limits>

std::vector<int> NEH::solve() {
    int n = problem.n;
    int m = problem.m;

    // Step 1: Compute total processing time for each job
    std::vector<std::pair<int, int>> jobProcessingTime;
    for (int j = 0; j < n; j++) {
        int totalProcessingTime = 0;
        for (int k = 0; k < m; k++) {
            totalProcessingTime += problem.processingTimes[j][k];
        }
        jobProcessingTime.emplace_back(totalProcessingTime, j);
    }

    // Step 2: Sort jobs in descending order of total processing time
    std::sort(jobProcessingTime.rbegin(), jobProcessingTime.rend());

    // Step 3: Construct the schedule incrementally
    std::vector<int> schedule;
    for (const auto &job : jobProcessingTime) {
        int jobID = job.second;
        int bestPos = bestInsertionPosition(schedule, jobID);
        schedule.insert(schedule.begin() + bestPos, jobID);
    }

    return schedule;
}

// Finds the best insertion position to minimize makespan
int NEH::bestInsertionPosition(const std::vector<int> &partialSchedule, int job) {
    int bestMakespan = std::numeric_limits<int>::max();
    int bestPos = 0;

    for (size_t pos = 0; pos <= partialSchedule.size(); pos++) {
        std::vector<int> tempSchedule = partialSchedule;
        tempSchedule.insert(tempSchedule.begin() + pos, job);

        int makespan = evaluateMakespan(problem, tempSchedule);
        if (makespan < bestMakespan) {
            bestMakespan = makespan;
            bestPos = pos;
        }
    }

    return bestPos;
}
