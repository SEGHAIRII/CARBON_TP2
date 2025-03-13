#include "NEHLS.h"
#include <algorithm>
#include <limits>
#include <chrono>
#include <iostream>
#include <numeric> // For std::iota

NEHLS::NEHLS(const Problem& prob) : problem(prob) {
    n_jobs = problem.processingTimes.size();
    n_machines = (n_jobs > 0) ? problem.processingTimes[0].size() : 0;
}

int NEHLS::computeMakespan(const std::vector<int>& sequence) {
    if (sequence.empty())
        return 0;
    
    std::vector<std::vector<int>> completion(sequence.size(), std::vector<int>(n_machines, 0));
    int first_job = sequence[0];
    completion[0][0] = problem.processingTimes[first_job][0];
    for (int m = 1; m < n_machines; m++) {
        completion[0][m] = completion[0][m-1] + problem.processingTimes[first_job][m];
    }
    for (int j = 1; j < sequence.size(); j++) {
        int job = sequence[j];
        completion[j][0] = completion[j-1][0] + problem.processingTimes[job][0];
        for (int m = 1; m < n_machines; m++) {
            completion[j][m] = std::max(completion[j][m-1], completion[j-1][m]) + problem.processingTimes[job][m];
        }
    }
    return completion[sequence.size()-1][n_machines-1];
}

std::vector<int> NEHLS::computeInitialJobOrder() {
    std::vector<int> order(n_jobs);
    std::iota(order.begin(), order.end(), 0);
    
    // Sort in descending order by total processing time across all machines
    std::sort(order.begin(), order.end(), [this](int a, int b) {
        int total_a = 0, total_b = 0;
        for (int m = 0; m < n_machines; m++) {
            total_a += problem.processingTimes[a][m];
            total_b += problem.processingTimes[b][m];
        }
        return total_a > total_b;
    });
    return order;
}

std::pair<int, int> NEHLS::findBestPosition(const std::vector<int>& partial_sequence, int job) {
    int best_makespan = std::numeric_limits<int>::max();
    int best_position = 0;
    for (int pos = 0; pos <= partial_sequence.size(); pos++) {
        std::vector<int> new_sequence = partial_sequence;
        new_sequence.insert(new_sequence.begin() + pos, job);
        int mks = computeMakespan(new_sequence);
        if (mks < best_makespan) {
            best_makespan = mks;
            best_position = pos;
        }
    }
    return {best_position, best_makespan};
}

std::vector<int> NEHLS::localSearch(const std::vector<int>& sequence) {
    bool improved = true;
    std::vector<int> best_sequence = sequence;
    int current_best = computeMakespan(best_sequence);
    
    while (improved) {
        improved = false;
        for (size_t i = 0; i < best_sequence.size(); i++) {
            for (size_t j = i + 1; j < best_sequence.size(); j++) {
                std::vector<int> new_sequence = best_sequence;
                std::swap(new_sequence[i], new_sequence[j]);
                int new_mks = computeMakespan(new_sequence);
                if (new_mks < current_best) {
                    best_sequence = new_sequence;
                    current_best = new_mks;
                    improved = true;
                    break;
                }
            }
            if (improved)
                break;
        }
    }
    return best_sequence;
}

std::vector<int> NEHLS::insertionLocalSearch(const std::vector<int>& sequence) {
    bool improved = true;
    std::vector<int> best_sequence = sequence;
    int current_best = computeMakespan(best_sequence);
    
    while (improved) {
        improved = false;
        for (size_t i = 0; i < best_sequence.size(); i++) {
            int current_job = best_sequence[i];
            std::vector<int> temp_sequence = best_sequence;
            temp_sequence.erase(temp_sequence.begin() + i);
            auto [best_pos, best_mks] = findBestPosition(temp_sequence, current_job);
            if (best_mks < current_best) {
                temp_sequence.insert(temp_sequence.begin() + best_pos, current_job);
                best_sequence = temp_sequence;
                current_best = best_mks;
                improved = true;
                break;
            }
        }
    }
    return best_sequence;
}

std::vector<int> NEHLS::solve() {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Compute initial job order using NEH sorting
    std::vector<int> job_order = computeInitialJobOrder();
    std::vector<int> sequence;
    sequence.push_back(job_order[0]);
    
    // Build initial sequence by inserting jobs at the best positions (NEH insertion step)
    for (int j = 1; j < n_jobs; j++) {
        int next_job = job_order[j];
        auto [pos, _] = findBestPosition(sequence, next_job);
        sequence.insert(sequence.begin() + pos, next_job);
    }
    
    // Apply pairwise exchange local search
    sequence = localSearch(sequence);
    // Apply insertion-based local search for further improvement
    sequence = insertionLocalSearch(sequence);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    double computation_time = std::chrono::duration<double>(end_time - start_time).count();
    
    std::cout << "NEHLS Makespan: " << computeMakespan(sequence) << std::endl;
    std::cout << "Computation Time: " << computation_time << " seconds" << std::endl;
    
    return sequence;
}
