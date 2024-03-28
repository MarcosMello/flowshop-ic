#include "cplexModel.h"
#include "GeneticAlgorithm.h"

string path = "../Code/data/equal_deadlines/n10m20.dat";

constexpr size_t mutationProbability = 10;
constexpr size_t individualTransferRate = 20;
constexpr size_t populationSize = 1000;

constexpr size_t maximumIterations = 100;
constexpr size_t maxIterationsWithoutImprovement = 10;

void parseArguments(const int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        if (string argument = argv[i]; argument == "-cplexVerbose") {
            isCplexVerboseActive = true;
        } else if(argument == "-printSolutions") {
            shouldPrintSolution = true;
        } else if (argument.find("-cplexTimeLimit=") == 0) {
            string strippedArgument = argument.substr(16);
            timeLimitInSeconds = static_cast<int>(strtol(strippedArgument.c_str(), nullptr, 10));
        } else if (argument.find("-path=") == 0) {
            path = argument.substr(6);
        }
    }
}

int main(const int argc, char *argv[]) {
    parseArguments(argc, argv);

    const auto data = InputData(path);

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
