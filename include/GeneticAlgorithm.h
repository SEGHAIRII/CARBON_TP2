#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "Heuristic.h"
#include "Problem.h"
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

class GeneticAlgorithm : public Heuristic {
public:
    GeneticAlgorithm(const Problem& prob, int popSize, int generations, double crossRate, double mutRate, int tourSize);
    std::vector<int> solve() override;

private:
    struct Individual {
        std::vector<int> permutation;
        int makespan;
    };

    const Problem& problem;
    int populationSize, generations, tournamentSize;
    double crossoverRate, mutationRate;
    std::vector<Individual> population;
    std::mt19937 rng;

    void initializePopulation();
    Individual tournamentSelection();
    std::pair<Individual, Individual> orderCrossover(const Individual& p1, const Individual& p2);
    void swapMutation(Individual& ind);
    static bool compare(const Individual& a, const Individual& b);
    double randomProb();
};

#endif // GENETIC_ALGORITHM_H
