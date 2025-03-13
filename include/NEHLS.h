#ifndef NEHLS_H
#define NEHLS_H

#include "Heuristic.h"
#include "Problem.h"
#include <vector>

class NEHLS : public Heuristic {
public:
    explicit NEHLS(const Problem& prob);
    // Implements the interface: returns the final job sequence (0-indexed)
    std::vector<int> solve() override;

private:
    const Problem& problem;
    int n_jobs;
    int n_machines;

    // Computes the makespan for a given job sequence using flow shop scheduling logic.
    int computeMakespan(const std::vector<int>& sequence);
    // Computes the initial job order by sorting jobs in descending order of total processing time.
    std::vector<int> computeInitialJobOrder();
    // For a given partial sequence, finds the best position to insert a new job and returns (position, makespan)
    std::pair<int, int> findBestPosition(const std::vector<int>& partial_sequence, int job);
    // Performs a local search using pairwise exchanges
    std::vector<int> localSearch(const std::vector<int>& sequence);
    // Performs an insertion-based local search
    std::vector<int> insertionLocalSearch(const std::vector<int>& sequence);
};

#endif // NEHLS_H
