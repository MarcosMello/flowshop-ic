#include "GeneticAlgorithm.h"

class GeneticAlgorithmRunner{
public:
    GeneticAlgorithmRunner(const size_t maximumIterations, const size_t maximumIterationsWithoutImprovement,
        const size_t mutationProbability, const size_t individualTransferRate,
        const size_t populationSize, const vector<vector<int>> &processingTime, const vector<int> &deadlines) :
        maximumIterations(maximumIterations), maximumIterationsWithoutImprovement(maximumIterationsWithoutImprovement),
        mutationProbability(mutationProbability), individualTransferRate(individualTransferRate),
        populationSize(populationSize), processingTime(processingTime), deadlines(deadlines),
        population(mutationProbability, individualTransferRate, populationSize, processingTime, deadlines){
        size_t objectiveValue = 0;

        while(iteration++ < maximumIterations && iterationsWithoutImprovement < maximumIterationsWithoutImprovement) {
            iterationsWithoutImprovement++;

            population.generateNextGeneration();

            if (this->getObjectiveValue() < objectiveValue) {
                iterationsWithoutImprovement = 0;
                objectiveValue = this->getObjectiveValue();
            }
        }
    }

    [[nodiscard]]
    size_t getObjectiveValue() const {
        return population.getObjectiveValue();
    }

    [[nodiscard]]
    vector<int> getObjectiveJobOrder() const {
        return population.getObjectiveJobOrder();
    }

private:
    size_t iteration = 0;

    size_t iterationsWithoutImprovement = 0;

    const size_t maximumIterations;
    const size_t maximumIterationsWithoutImprovement;

    const size_t mutationProbability;
    const size_t individualTransferRate;
    const size_t populationSize;

    const vector<vector<int>> &processingTime;
    const vector<int> &deadlines;

    Population population;
};

int main(int argc, char *argv[]) {
    const auto data = InputData("../Code/data/equal_deadlines/n10m20.dat");

    const vector<vector<int>> &processingTime = data.processingTime;
    const vector<int> &deadlines = data.deadlines;

    constexpr size_t mutationProbability = 10;
    constexpr size_t individualTransferRate = 20;
    constexpr size_t populationSize = 1000;

    constexpr size_t maximumIterations = 100;
    constexpr size_t maxIterationsWithoutImprovement = 10;

    const GeneticAlgorithmRunner runner(
        maximumIterations,
        maxIterationsWithoutImprovement,
        mutationProbability,
        individualTransferRate,
        populationSize,
        processingTime,
        deadlines
    );

    cout << "Objective Value: " << runner.getObjectiveValue() << endl;

    cout << "Job order: ";
    for (const auto job: runner.getObjectiveJobOrder()) {
        cout << job << " ";
    }
    cout << endl;

    return 0;
}
