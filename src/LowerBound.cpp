#include "LowerBounds.h"
#include <algorithm>
#include <climits>
#include <vector>

// Simulate the processing of the partial schedule and return completion times on each machine.
std::vector<int> computeScheduleCompletion(const Problem &prob, const std::vector<int> &schedule) {
    int m = prob.m;
    std::vector<int> completion(m, 0);
    // For each job in the partial schedule:
    for (int job : schedule) {
        for (int machine = 0; machine < m; machine++) {
            if (machine == 0)
                completion[0] += prob.processingTimes[job][0];
            else
                completion[machine] = std::max(completion[machine], completion[machine - 1]) + prob.processingTimes[job][machine];
        }
    }
    return completion;
}

int computeTailMin(const Problem &prob, const std::vector<bool> &scheduled, int machine) {
    int m = prob.m;
    int minSum = INT_MAX;
    bool found = false;
    for (int j = 0; j < prob.n; j++) {
        if (!scheduled[j]) {
            int sum = 0;
            for (int l = machine + 1; l < m; l++) {
                sum += prob.processingTimes[j][l];
            }
            if (sum < minSum)
                minSum = sum;
            found = true;
        }
    }
    return found ? minSum : 0;
}

int computeSumForMachine(const Problem &prob, const std::vector<bool> &scheduled, int machine) {
    int sum = 0;
    for (int j = 0; j < prob.n; j++) {
        if (!scheduled[j]) {
            sum += prob.processingTimes[j][machine];
        }
    }
    return sum;
}

int computeLB1(const Problem &prob, const std::vector<int> &schedule, const std::vector<bool> &scheduled) {
    std::vector<int> r = computeScheduleCompletion(prob, schedule);
    int lb = 0;
    for (int machine = 0; machine < prob.m; machine++) {
        int pSum = computeSumForMachine(prob, scheduled, machine);
        int tail = computeTailMin(prob, scheduled, machine);
        int candidate = r[machine] + pSum + tail;
        lb = std::max(lb, candidate);
    }
    return lb;
}

// Data structure used in Johnson's algorithm.
struct JohnsonJob {
    int jobIndex;
    int p_k; // Processing time on machine k
    int p_l; // Processing time on machine l
};

int computeJohnsonMakespan(const Problem &prob, const std::vector<bool> &scheduled, int machineK, int machineL) {
    std::vector<JohnsonJob> jobs;
    for (int j = 0; j < prob.n; j++) {
        if (!scheduled[j]) {
            JohnsonJob job;
            job.jobIndex = j;
            job.p_k = prob.processingTimes[j][machineK];
            job.p_l = prob.processingTimes[j][machineL];
            jobs.push_back(job);
        }
    }
    // Apply Johnson's rule:
    std::vector<JohnsonJob> groupA, groupB;
    for (auto &job : jobs) {
        if (job.p_k <= job.p_l)
            groupA.push_back(job);
        else
            groupB.push_back(job);
    }
    std::sort(groupA.begin(), groupA.end(), [](const JohnsonJob &a, const JohnsonJob &b) {
        return a.p_k < b.p_k;
    });
    std::sort(groupB.begin(), groupB.end(), [](const JohnsonJob &a, const JohnsonJob &b) {
        return a.p_l > b.p_l;
    });
    std::vector<JohnsonJob> sequence;
    sequence.insert(sequence.end(), groupA.begin(), groupA.end());
    sequence.insert(sequence.end(), groupB.begin(), groupB.end());
    
    // Compute the makespan for the two-machine problem.
    int completionK = 0, completionL = 0;
    for (auto &job : sequence) {
        completionK += job.p_k;
        completionL = std::max(completionK, completionL) + job.p_l;
    }
    return completionL;
}

int computeLB2(const Problem &prob, const std::vector<int> &schedule, const std::vector<bool> &scheduled) {
    std::vector<int> r = computeScheduleCompletion(prob, schedule);
    int lb2 = 0;
    int m = prob.m;
    for (int k = 0; k < m; k++) {
        for (int l = k + 1; l < m; l++) {
            int johnsonMakespan = computeJohnsonMakespan(prob, scheduled, k, l);
            int tail = computeTailMin(prob, scheduled, l);
            int candidate = r[k] + johnsonMakespan + tail;
            lb2 = std::max(lb2, candidate);
        }
    }
    return lb2;
}
