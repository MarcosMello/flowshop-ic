#include "GeneticAlgorithm.h"

Individual::Individual(const vector<vector<int>> &processingTime, const vector<int> &deadlines,
    vector<int> value) : value(std::move(value)), fitness(0) {
    calculate_fitness(processingTime, deadlines);
}

Individual::Individual(const vector<vector<int>> &processingTime, const vector<int> &deadlines) :
    Individual(processingTime, deadlines, getRandomJobPermutation(deadlines.size())) {}

vector<int> Individual::getValue() const {
    return this->value;
}

size_t Individual::getFitness() const {
    return this->fitness;
}

size_t Individual::size() const {
    return this->value.size();
}

vector<int> Individual::createChildValueVector(const Individual &parent) const {
    vector childValue(this->size(), -1);
    unordered_set<int> childUniqueValues;

    const size_t parentGenesStart = getRandomNumber(0, childValue.size() - 2);
    const size_t parentGenesEnd = getRandomNumber(parentGenesStart, childValue.size() - 1);

    for (size_t i = 0; i < childValue.size(); i++) {
        if (i == parentGenesStart) {
            i = parentGenesEnd;
        }

        childValue[i] = this->value[i];
        childUniqueValues.insert(this->value[i]);
    }

    size_t parentGenesIndex = parentGenesStart;

    for (size_t i = 0; i < parent.size(); i++) {
        if (childUniqueValues.find(parent[i]) == childUniqueValues.end()) {
            childValue[parentGenesIndex] = parent[i];
            parentGenesIndex++;
        }
    }

    return childValue;
}

void Individual::mutate() {
    const size_t slidingGenesStart = getRandomNumber(0, this->size() - 2);
    const size_t slidingGenesEnd = getRandomNumber(slidingGenesStart + 1, this->size() - 1);

    const int slidedGene = this->value[slidingGenesEnd];

    for (size_t i = slidingGenesEnd; i > slidingGenesStart; i--) {
        this->value[i] = this->value[i - 1];
    }

    this->value[slidingGenesStart] = slidedGene;
}

const int &Individual::back() const {
    return this->value.back();
}

void Individual::calculate_fitness(const vector<vector<int>> &processingTime, const vector<int> &deadlines) {
    fitness = Conclusion(processingTime, deadlines, this).getObjectiveValue();
}