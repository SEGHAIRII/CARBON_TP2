#include "BranchAndBound.h"
#include "LowerBounds.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <future>
#include <vector>

BranchAndBound::BranchAndBound(const Problem &prob, int parallelDepth)
    : problem(prob), parallelDepth(parallelDepth), bestMakespan(std::numeric_limits<int>::max()) {}

int BranchAndBound::evaluateMakespan(const std::vector<int> &schedule) {
    // Compute the makespan of a complete schedule using the standard flow-shop formula.
    int m = problem.m;
    int n = schedule.size();
    std::vector<int> completion(m, 0);
    for (int i = 0; i < n; i++) {
        int job = schedule[i];
        for (int machine = 0; machine < m; machine++) {
            if (machine == 0) {
                completion[0] += problem.processingTimes[job][0];
            } else {
                completion[machine] = std::max(completion[machine], completion[machine - 1]) + problem.processingTimes[job][machine];
            }
        }
    }
    return completion[m - 1];
}

void BranchAndBound::branch(const Node &node) {
    // If a complete schedule is reached, evaluate and update the best makespan.
    if (node.schedule.size() == static_cast<size_t>(problem.n)) {
        int makespan = evaluateMakespan(node.schedule);
        {
            std::lock_guard<std::mutex> lock(bestMutex);
            if (makespan < bestMakespan) {
                bestMakespan = makespan;
                bestSchedule = node.schedule;
                std::cout << "Best schedule: ";
                for (int job : bestSchedule) {
                    std::cout << job << " ";
                }
                std::cout << "New best makespan: " << bestMakespan << "\n";
            }
        }
        return;
    }
    
    // Compute LB1 and prune if not promising.
    int lb1 = computeLB1(problem, node.schedule, node.scheduled);
    {
        std::lock_guard<std::mutex> lock(bestMutex);
        if (lb1 >= bestMakespan)
            return;
    }
    
    // Compute LB2 for a tighter bound.
    int lb2 = computeLB2(problem, node.schedule, node.scheduled);
    {
        std::lock_guard<std::mutex> lock(bestMutex);
        if (lb2 >= bestMakespan)
            return;
    }
    
    // Static forward branching: extend the schedule with every unscheduled job.
    std::vector<std::future<void>> futures;
    for (int j = 0; j < problem.n; j++) {
        if (!node.scheduled[j]) {
            Node child;
            child.schedule = node.schedule;
            child.schedule.push_back(j);
            child.scheduled = node.scheduled;
            child.scheduled[j] = true;
            child.depth = node.depth + 1;
            
            // Spawn parallel tasks at the upper levels.
            if (node.depth < parallelDepth) {
                futures.push_back(std::async(std::launch::async, &BranchAndBound::branch, this, child));
            } else {
                branch(child);
            }
        }
    }
    // Wait for all parallel tasks to finish.
    for (auto &fut : futures) {
        fut.get();
    }
}

void BranchAndBound::run() {
    // Initialize the root node with an empty schedule.
    Node root;
    root.schedule = std::vector<int>();
    root.scheduled = std::vector<bool>(problem.n, false);
    root.depth = 0;
    branch(root);
    
    std::cout << "Best makespan found: " << bestMakespan << "\n";
    std::cout << "Best schedule: ";
    for (int job : bestSchedule) {
        std::cout << job << " ";
    }
    std::cout << "\n";
}
