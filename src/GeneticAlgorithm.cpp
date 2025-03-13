#include "GeneticAlgorithm.h"
#include <iostream>
#include <tuple>
#include <numeric>  // For std::iota

GeneticAlgorithm::GeneticAlgorithm(const Problem& prob, int popSize, int generations, double crossRate, double mutRate, int tourSize)
    : problem(prob), populationSize(popSize), generations(generations), crossoverRate(crossRate), mutationRate(mutRate), tournamentSize(tourSize),
      rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

std::vector<int> GeneticAlgorithm::solve() {
    initializePopulation();
    Individual bestIndividual = *std::min_element(population.begin(), population.end(), compare);
    
    for (int gen = 0; gen < generations; ++gen) {
        std::vector<Individual> newPopulation;
        newPopulation.push_back(bestIndividual);  // Elitism
        
        while (newPopulation.size() < populationSize) {
            Individual parent1 = tournamentSelection();
            Individual parent2 = tournamentSelection();
            Individual offspring1, offspring2;
            
            if (randomProb() < crossoverRate) {
                std::tie(offspring1, offspring2) = orderCrossover(parent1, parent2);
            } else {
                offspring1 = parent1;
                offspring2 = parent2;
            }
            
            if (randomProb() < mutationRate) swapMutation(offspring1);
            if (randomProb() < mutationRate) swapMutation(offspring2);
            
            offspring1.makespan = evaluateMakespan(problem, offspring1.permutation);
            offspring2.makespan = evaluateMakespan(problem, offspring2.permutation);
            
            newPopulation.push_back(offspring1);
            if (newPopulation.size() < populationSize) newPopulation.push_back(offspring2);
        }
        
        population = newPopulation;
        bestIndividual = *std::min_element(population.begin(), population.end(), compare);
        if (gen % 50 == 0) {
            std::cout << "Generation " << gen << " Best Makespan: " << bestIndividual.makespan << std::endl;
        }
    }
    return bestIndividual.permutation;
}

void GeneticAlgorithm::initializePopulation() {
    int numJobs = problem.processingTimes.size();
    std::vector<int> basePermutation(numJobs);
    std::iota(basePermutation.begin(), basePermutation.end(), 0);
    
    for (int i = 0; i < populationSize; ++i) {
        Individual ind;
        ind.permutation = basePermutation;
        std::shuffle(ind.permutation.begin(), ind.permutation.end(), rng);
        ind.makespan = evaluateMakespan(problem, ind.permutation);
        population.push_back(ind);
    }
}

GeneticAlgorithm::Individual GeneticAlgorithm::tournamentSelection() {
    std::uniform_int_distribution<> dist(0, population.size() - 1);
    Individual best = population[dist(rng)];
    for (int i = 1; i < tournamentSize; i++) {
        Individual competitor = population[dist(rng)];
        if (competitor.makespan < best.makespan) best = competitor;
    }
    return best;
}

std::pair<GeneticAlgorithm::Individual, GeneticAlgorithm::Individual> GeneticAlgorithm::orderCrossover(const Individual& p1, const Individual& p2) {
    int n = p1.permutation.size();
    std::uniform_int_distribution<> dist(0, n - 1);
    int start = dist(rng), end = dist(rng);
    if (start > end) std::swap(start, end);
    
    Individual o1, o2;
    o1.permutation.resize(n, -1);
    o2.permutation.resize(n, -1);
    
    for (int i = start; i <= end; i++) {
        o1.permutation[i] = p1.permutation[i];
        o2.permutation[i] = p2.permutation[i];
    }
    
    auto fillOffspring = [&](const Individual& donor, Individual& offspring) {
        int current = (end + 1) % n;
        for (int i = 0; i < n; i++) {
            int idx = (end + 1 + i) % n;
            if (std::find(offspring.permutation.begin(), offspring.permutation.end(), donor.permutation[idx]) == offspring.permutation.end()) {
                offspring.permutation[current] = donor.permutation[idx];
                current = (current + 1) % n;
            }
        }
    };
    
    fillOffspring(p2, o1);
    fillOffspring(p1, o2);
    
    return {o1, o2};
}

void GeneticAlgorithm::swapMutation(Individual& ind) {
    int n = ind.permutation.size();
    std::uniform_int_distribution<> dist(0, n - 1);
    int i = dist(rng), j = dist(rng);
    std::swap(ind.permutation[i], ind.permutation[j]);
}

bool GeneticAlgorithm::compare(const Individual& a, const Individual& b) {
    return a.makespan < b.makespan;
}

double GeneticAlgorithm::randomProb() {
    std::uniform_real_distribution<> prob(0.0, 1.0);
    return prob(rng);
}
