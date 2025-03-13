#include "PalmersHeuristic.h"
#include <algorithm>

std::vector<int> PalmersHeuristic::solve() {
    int n = problem.n, m = problem.m;
    std::vector<std::pair<int, int>> priorityIndex;

    for (int j = 0; j < n; j++) {
        int priority = 0;
        for (int k = 0; k < m; k++) {
            priority += (m - 2 * k - 1) * problem.processingTimes[j][k];
        }
        priorityIndex.emplace_back(-priority, j);
    }

    std::sort(priorityIndex.begin(), priorityIndex.end());

    std::vector<int> schedule;
    for (const auto &p : priorityIndex) {
        schedule.push_back(p.second);
    }
    return schedule;
}
