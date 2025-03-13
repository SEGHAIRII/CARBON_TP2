#include "RajendranChaudhuri.h"
#include <algorithm>
#include <limits>

std::vector<int> RajendranChaudhuri::solve() {
    int n = problem.n;
    int m = problem.m;
    double alpha = 0.5; // Weight factor for first and last machine times

    // Step 1: Compute modified priority index for each job
    std::vector<std::pair<double, int>> jobPriority;
    for (int j = 0; j < n; j++) {
        double priority = 0;
        for (int k = 0; k < m; k++) {
            priority += problem.processingTimes[j][k];
        }
        priority += alpha * (problem.processingTimes[j][0] + problem.processingTimes[j][m - 1]);
        jobPriority.emplace_back(-priority, j);  // Negative for descending order
    }

    // Step 2: Sort jobs by descending priority index
    std::sort(jobPriority.begin(), jobPriority.end());

    // Step 3: Incrementally construct the schedule
    std::vector<int> schedule;
    for (const auto &job : jobPriority) {
        int jobID = job.second;
        int bestPos = bestInsertionPosition(schedule, jobID);
        schedule.insert(schedule.begin() + bestPos, jobID);
    }

    return schedule;
}

// Finds the best insertion position to minimize idle time & makespan
int RajendranChaudhuri::bestInsertionPosition(const std::vector<int> &partialSchedule, int job) {
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
