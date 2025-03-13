#include "Heuristic.h"
#include <algorithm>

// Compute makespan for a given job sequence
int Heuristic::evaluateMakespan(const Problem &problem, const std::vector<int> &schedule) {
    int m = problem.m;
    int n = schedule.size();
    std::vector<std::vector<int>> completion(n, std::vector<int>(m, 0));

    for (int i = 0; i < n; i++) {
        int job = schedule[i];
        for (int machine = 0; machine < m; machine++) {
            if (i == 0 && machine == 0)
                completion[i][machine] = problem.processingTimes[job][machine];
            else if (i == 0)
                completion[i][machine] = completion[i][machine - 1] + problem.processingTimes[job][machine];
            else if (machine == 0)
                completion[i][machine] = completion[i - 1][machine] + problem.processingTimes[job][machine];
            else
                completion[i][machine] = std::max(completion[i - 1][machine], completion[i][machine - 1]) + problem.processingTimes[job][machine];
        }
    }
    return completion[n - 1][m - 1];  // Return the makespan
}
