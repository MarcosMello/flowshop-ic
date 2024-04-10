#include "../GeneticAlgorithm.h"

TabuSearch::TabuSearch(const vector<vector<int>> &processingTime, const vector<int> &deadlines,
    const Individual &individual) : processingTime(processingTime), deadlines(deadlines) {

    Individual bestIndividual = individual;
    Individual currentIndividual = individual;
    vector<vector<int>> tabuList;

    for (size_t i = 0; i < maxIterations; i++) {
        vector<Individual> neighbors = getNeighbors(currentIndividual);
        const Individual *bestNeighbor = nullptr;

        size_t bestNeighborFitness = std::numeric_limits<size_t>::max();

        for (Individual &neighbor : neighbors) {
            if (find(tabuList.begin(), tabuList.end(), neighbor.getValue()) == tabuList.end()) {

                if (neighbor.getFitness() < bestNeighborFitness) {
                    bestNeighbor = &neighbor;
                    bestNeighborFitness = neighbor.getFitness();
                }
            }
        }

        if (bestNeighbor == nullptr) {
            break;
        }

        currentIndividual = *bestNeighbor;
        tabuList.emplace_back(bestNeighbor->getValue());
        if (tabuList.size() > tabuListSize) {
            tabuList.erase(tabuList.begin());
        }

        if (bestNeighbor->getFitness() < bestIndividual.getFitness()) {
            bestIndividual = *bestNeighbor;
        }
    }

    this->bestIndividualValue = bestIndividual.getValue();
}

vector<Individual> TabuSearch::getNeighbors(const Individual &individual) const {
    vector<Individual> neighbors;

    for (size_t i = 0; i < individual.size(); i++) {
        for (size_t j = i + 1; j < individual.size(); j++) {
            vector neighborValue = individual.getValue();

            swap(neighborValue[i], neighborValue[j]);

            neighbors.emplace_back(this->processingTime, this->deadlines, neighborValue);
        }
    }

    return neighbors;
}