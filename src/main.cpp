#include "Problem.h"
#include "PalmersHeuristic.h"
#include "NEH.h"
#include "CDS.h"
#include "RajendranChaudhuri.h"
#include "MNEH.h"
#include <iostream>
#include <memory>
#include <chrono>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <heuristic> <input_file>\n";
        std::cout << "Available heuristics: palmer, neh, cds, rc, mneh\n";
        return 1;
    }

    std::string heuristicType = argv[1];
    Problem prob;
    if (!prob.loadFromFile(argv[2])) {
        std::cerr << "Failed to load problem file.\n";
        return 1;
    }

    std::unique_ptr<Heuristic> heuristic;

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
    } else {
        std::cerr << "Unknown heuristic: " << heuristicType << "\n";
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto schedule = heuristic->solve();
    int makespan = heuristic->evaluateMakespan(prob, schedule);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Best makespan found: " << makespan << "\nBest schedule: ";
    for (int job : schedule) std::cout << job << " ";
    std::cout << "\nExecution Time: " << elapsed.count() << " seconds\n";

    return 0;
}
