#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "inputData.h"

inline size_t maxIterations = 100;
inline size_t tabuListSize = 10;

constexpr int NUMBER_OF_PARENTS = 2;
constexpr int SEED = 1;

inline seed_seq seed{SEED};
inline auto defaultRandomEngine = default_random_engine(seed);

inline auto getRandomNumber(const size_t lowerBound, const size_t upperBound) {
    return uniform_int_distribution (lowerBound, upperBound) (defaultRandomEngine);
}

inline vector<int> getRandomJobPermutation(const size_t length) {
    vector<int> jobs(length);

    iota(jobs.begin(), jobs.end(), 0);
    shuffle(jobs.begin(), jobs.end(), defaultRandomEngine);

    return jobs;
}

class Individual{
public:
    Individual(const vector<vector<int>> &processingTime, const vector<int> &deadlines, vector<int> value);
    Individual(const vector<vector<int>> &processingTime, const vector<int> &deadlines);

    [[nodiscard]] vector<int> getValue() const;
    [[nodiscard]] size_t getFitness() const;
    [[nodiscard]] size_t size() const;

    [[nodiscard]] vector<int> createChildValueVector(const Individual &parent) const;
    void mutate();

    [[nodiscard]] const int &back() const;

    auto operator[](const size_t i) const{
        return this->value[i];
    }
    auto operator<(const Individual &rhs) const{
        return (this->fitness < rhs.getFitness());
    }

private:
    vector<int> value;
    size_t fitness;

    void calculate_fitness(const vector<vector<int>> &processingTime, const vector<int> &deadlines);
};

class Population {
public:
    Population(size_t mutationProbability,
               size_t individualTransferRate,
               size_t populationSize,
               const vector<vector<int>> &processingTime,
               const vector<int> &deadlines);

    Population(size_t mutationProbability, size_t individualTransferRate, const vector<vector<int>> &processingTime,
        const vector<int> &deadlines, const vector<Individual>& population);

    void generateNextGeneration();

    [[nodiscard]] const Individual &front() const;

    [[nodiscard]] size_t getObjectiveValue() const;

    [[nodiscard]] vector<int> getObjectiveJobOrder() const;

    void changeLeastFittest(const Individual &individual);

    void sort();

private:
    size_t generation;

    size_t mutationProbability;

    size_t individualTransferRate;
    size_t populationSize;

    vector<Individual> population;

    vector<vector<int>> processingTime;
    vector<int> deadlines;
};

class Conclusion {
public:
    Conclusion(const vector<vector<int>> &processingTime, const vector<int> &deadlines, Individual *individual);

    [[nodiscard]] vector<vector<int>> generateConclusion() const;

    [[nodiscard]] int getObjectiveValue() const;
private:
    vector<vector<int>> processingTime;
    vector<int> deadlines;
    Individual *individual;

    size_t jobs;
    size_t machines;

    vector<vector<int>> conclusion;
    int objectiveValue;
};

class GeneticAlgorithmRunner{
public:
    GeneticAlgorithmRunner(size_t maximumIterations, size_t maximumIterationsWithoutImprovement,
        size_t mutationProbability, size_t individualTransferRate, size_t populationSize,
        const vector<vector<int>> &processingTime, const vector<int> &deadlines);

    [[nodiscard]] size_t getObjectiveValue() const;

    [[nodiscard]] vector<int> getObjectiveJobOrder() const;
    [[nodiscard]] chrono::duration<double> getElapsedTime() const;

    void print() const;

private:
    size_t iteration = 0;

    size_t iterationsWithoutImprovement = 0;

    Population population;

    chrono::duration<double> timeSpent = chrono::duration<double>();
};

class TabuSearch {
public:
    TabuSearch(const vector<vector<int>> &processingTime, const vector<int> &deadlines, const Individual &individual);

    [[nodiscard]]
    vector<Individual> getNeighbors(const Individual &individual) const;

    [[nodiscard]]
    vector<int> getBestIndividualValue() {
        return this->bestIndividualValue;
    }
private:
    const vector<vector<int>> &processingTime;
    const vector<int> &deadlines;

    vector<int> bestIndividualValue;
};

#endif //GENETICALGORITHM_H
