#include "MNEH.h"
#include <algorithm>
#include <limits>

std::vector<int> MNEH::solve() {
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

    // Step 3: Construct the schedule incrementally using improved insertion strategy
    std::vector<int> schedule;
    for (const auto &job : jobProcessingTime) {
        int jobID = job.second;
        int bestPos = bestInsertionPosition(schedule, jobID);
        schedule.insert(schedule.begin() + bestPos, jobID);
    }

    return schedule;
}

// Find the best insertion position to minimize makespan and balance workload
int MNEH::bestInsertionPosition(const std::vector<int> &partialSchedule, int job) {
    int bestMakespan = std::numeric_limits<int>::max();
    int bestPos = 0;
    int bestIdleTime = std::numeric_limits<int>::max();

    for (size_t pos = 0; pos <= partialSchedule.size(); pos++) {
        std::vector<int> tempSchedule = partialSchedule;
        tempSchedule.insert(tempSchedule.begin() + pos, job);

        int makespan = evaluateMakespan(problem, tempSchedule);
        int idleTime = makespan - std::accumulate(problem.processingTimes[job].begin(), problem.processingTimes[job].end(), 0);

        // Prioritize lower makespan first, then lower idle time
        if (makespan < bestMakespan || (makespan == bestMakespan && idleTime < bestIdleTime)) {
            bestMakespan = makespan;
            bestIdleTime = idleTime;
            bestPos = pos;
        }
    }

    return bestPos;
}
