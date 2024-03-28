#include "cplexModel.h"
#include "GeneticAlgorithm.h"

constexpr size_t mutationProbability = 10;
constexpr size_t individualTransferRate = 20;
constexpr size_t populationSize = 1000;

constexpr size_t maximumIterations = 100;
constexpr size_t maxIterationsWithoutImprovement = 10;

int main(int argc, char *argv[]) {
    const auto data = InputData("../Code/data/equal_deadlines/n10m20.dat");

    cout << "Cplex: " << endl;
    cplexModelSolver(data);

    cout << "\nGenetic Algorithm: " << endl;
    const vector<vector<int>> &processingTime = data.processingTime;
    const vector<int> &deadlines = data.deadlines;

    const GeneticAlgorithmRunner runner(
        maximumIterations,
        maxIterationsWithoutImprovement,
        mutationProbability,
        individualTransferRate,
        populationSize,
        processingTime,
        deadlines
    );

    runner.print();

    return 0;
}
