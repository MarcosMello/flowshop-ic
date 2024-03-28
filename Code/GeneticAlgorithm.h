#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "inputData.h"

constexpr int NUMBER_OF_PARENTS = 2;

inline auto defaultRandomEngine = default_random_engine(random_device()());

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

    [[nodiscard]] vector<vector<int>> generateConclusion();

    [[nodiscard]] int getObjectiveValue() const;
private:
    vector<vector<int>> processingTime;
    vector<int> deadlines;
    Individual *individual;

    size_t jobs;
    size_t machines;

    vector<vector<int>> conclusion;
    int objectiveValue;

    [[nodiscard]] bool isJobEarly(size_t i) const;
    [[nodiscard]] int getLastTaskStartingTime(size_t i) const;
    [[nodiscard]] bool isNextJobProcessedAfterJobDeadline(size_t i) const;
    [[nodiscard]] bool isNextJobDependentOnJobConclusion(size_t i) const;
    [[nodiscard]] size_t nextJobProcessedBeforeJobDeadline(size_t outerI);
    [[nodiscard]] size_t earlyTaskVerificationFunction(size_t i);
};

#endif //GENETICALGORITHM_H
