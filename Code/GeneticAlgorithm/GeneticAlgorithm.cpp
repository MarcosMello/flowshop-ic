#include "../GeneticAlgorithm.h"

GeneticAlgorithmRunner::GeneticAlgorithmRunner(const size_t maximumIterations, const size_t maximumIterationsWithoutImprovement,
    const size_t mutationProbability, const size_t individualTransferRate,
    const size_t populationSize, const vector<vector<int>> &processingTime, const vector<int> &deadlines) :
    population(mutationProbability, individualTransferRate, populationSize, processingTime, deadlines){
    const auto timerStart = chrono::system_clock::now();

    size_t objectiveValue = 0;

    while(iteration++ < maximumIterations && iterationsWithoutImprovement < maximumIterationsWithoutImprovement) {
        iterationsWithoutImprovement++;

        population.generateNextGeneration();

        if (this->getObjectiveValue() < objectiveValue) {
            iterationsWithoutImprovement = 0;
            objectiveValue = this->getObjectiveValue();
        }

        if (iterationsWithoutImprovement == maximumIterationsWithoutImprovement/2) {
            const auto newIndividualValue =
                TabuSearch(processingTime, deadlines, population.front()).getBestIndividualValue();

            population.changeLeastFittest(Individual(processingTime, deadlines, newIndividualValue));
        }
    }

    const auto timerEnd = chrono::system_clock::now();

    this->timeSpent = timerEnd - timerStart;
}

[[nodiscard]]
size_t GeneticAlgorithmRunner::getObjectiveValue() const {
    return population.getObjectiveValue();
}

[[nodiscard]]
vector<int> GeneticAlgorithmRunner::getObjectiveJobOrder() const {
    return population.getObjectiveJobOrder();
}

[[nodiscard]]
chrono::duration<double> GeneticAlgorithmRunner::getElapsedTime() const {
    return this->timeSpent;
}

void GeneticAlgorithmRunner::print() const {
    cout << "Objective Value: " << this->getObjectiveValue() << endl;

    cout << "Job order: ";
    for (const auto job: this->getObjectiveJobOrder()) {
        cout << (job + 1) << " ";
    }
    cout << endl;

    cout << "Time spent: " << timeSpent.count() << endl;
}