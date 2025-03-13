#include "Problem.h"
#include "PalmersHeuristic.h"
#include "NEH.h"
#include "CDS.h"
#include "RajendranChaudhuri.h"
#include "MNEH.h"
#include "GeneticAlgorithm.h"
#include "NEHLS.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <string>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " input_file heuristic_type\n";
        std::cout << "Available heuristics: palmer, neh, cds, rc, mneh, ga, nehls\n";
        return 1;
    }
    
    Problem prob;
    if (!prob.loadFromFile(argv[1])) {
        std::cerr << "Failed to load problem file.\n";
        return 1;
    }
    
    std::unique_ptr<Heuristic> heuristic;
    std::string heuristicType = argv[2];
    
    if (heuristicType == "palmer") {
        heuristic = std::make_unique<PalmersHeuristic>(prob);
    } else if (heuristicType == "neh") {
        heuristic = std::make_unique<NEH>(prob);
    } else if (heuristicType == "cds") {
        heuristic = std::make_unique<CDS>(prob);
    } else if (heuristicType == "rc") {
        heuristic = std::make_unique<RajendranChaudhuri>(prob);
    } else if (heuristicType == "mneh") {
        heuristic = std::make_unique<MNEH>(prob);
    } else if (heuristicType == "ga") {
        heuristic = std::make_unique<GeneticAlgorithm>(prob, 50, 500, 0.8, 0.2, 5);
    } else if (heuristicType == "nehls") {
        heuristic = std::make_unique<NEHLS>(prob);
    } else {
        std::cerr << "Unknown heuristic type.\n";
        return 1;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> solution = heuristic->solve();
    auto end = std::chrono::high_resolution_clock::now();
    
    int makespan = heuristic->evaluateMakespan(prob, solution);
    
    std::cout << "Solution: ";
    for (int job : solution) {
        std::cout << job << " ";
    }
    std::cout << "\nBest Makespan: " << makespan << "\n";
    std::cout << "Execution Time: " << std::chrono::duration<double>(end - start).count() << " seconds\n";
    
    return 0;
}
