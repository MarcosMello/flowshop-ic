#include "cplexModel.h"
#include "NEH.h"
#include "GeneticAlgorithm.h"
#include "Latex/ComparisonTable.h"

#define FASTIO ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

string path = "../Code/data/equal_deadlines";

size_t mutationProbability = 40;
size_t individualTransferRate = 10;
size_t populationSize = 20;

size_t maximumIterations = 100;
size_t maxIterationsWithoutImprovement = 50;

string caption = "CAPTION HERE";
string label = "LABEL HERE";

bool useCPLEX = true;
bool useOnlyCPLEX = false;

void parseArguments(const int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        if (string argument = argv[i]; argument == "-cplexVerbose") {
            isCplexVerboseActive = true;
        } else if(argument == "-printSolutions") {
             shouldPrintSolution = true;
        } else if(argument == "-dontPrintTable") {
            shouldPrintTable = false;
        } else if (argument.find("-cplexTimeLimit=") == 0) {
            string strippedArgument = argument.substr(16);
            timeLimitInSeconds = static_cast<int>(strtol(strippedArgument.c_str(), nullptr, 10));
        } else if (argument.find("-cplexThreadsLimit=") == 0) {
            string strippedArgument = argument.substr(16);
            threadLimit = static_cast<int>(strtol(strippedArgument.c_str(), nullptr, 10));
        } else if (argument == "-hasObjectiveValueReference") {
            hasObjectiveValueReference = true;
        } else if (argument.find("-path=") == 0) {
            path = argument.substr(6);
        } else if (argument.find("-caption=") == 0) {
            caption = argument.substr(9);
        } else if (argument.find("-label=") == 0) {
            label = argument.substr(7);
        } else if (argument.find("-mutationProbability=") == 0) {
            string strippedArgument = argument.substr(21);
            mutationProbability = static_cast<size_t>(strtol(strippedArgument.c_str(), nullptr, 10));
        } else if (argument.find("-individualTransferRate=") == 0) {
            string strippedArgument = argument.substr(24);
            individualTransferRate = static_cast<size_t>(strtol(strippedArgument.c_str(), nullptr, 10));
        } else if (argument.find("-populationSize=") == 0) {
            string strippedArgument = argument.substr(16);
            populationSize = static_cast<size_t>(strtol(strippedArgument.c_str(), nullptr, 10));
        } else if (argument.find("-maximumIterations=") == 0) {
            string strippedArgument = argument.substr(19);
            maximumIterations = static_cast<size_t>(strtol(strippedArgument.c_str(), nullptr, 10));
        } else if (argument.find("-maxIterationsWithoutImprovement=") == 0) {
            string strippedArgument = argument.substr(33);
            maxIterationsWithoutImprovement = static_cast<size_t>(strtol(strippedArgument.c_str(), nullptr, 10));
        } else if (argument == "-noCplex") {
            useCPLEX = false;
        } else if (argument == "-useOnlyCPLEX") {
            isCplexVerboseActive = true;
            shouldPrintSolution = true;
            useOnlyCPLEX = true;
            useCPLEX = true;
        } else if (argument == "-geneticAlgorithmLog") {
            geneticAlgorithmLog = true;
        } else if (argument == "-noHeuristic") {
            runNEHAlgorithm = false;
        } else if (argument == "-unfixSeed") {
            auto timeNow = time(nullptr);

            cout << timeNow << endl;

            seed_seq seed{timeNow};
            defaultRandomEngine = default_random_engine(seed);
        }
    }
}

vector<string> generateFilesPathsVector(const string &path) {
    vector<string> filesPaths;

    if (is_directory(filesystem::status(path))){
        for (const auto& dirEntry : filesystem::recursive_directory_iterator(path)) {
            filesPaths.push_back(dirEntry.path().generic_string());
        }

        return filesPaths;
    }

    filesPaths.push_back(path);

    return filesPaths;
}

std::string currentDateTime() {
    const time_t now = time(nullptr);
    struct tm  tstruct{};
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int main(const int argc, char *argv[]) {
    FASTIO

    parseArguments(argc, argv);

    auto latexTable = Table(caption, label, hasObjectiveValueReference);

    const vector<string> filesPaths =  generateFilesPathsVector(path);
    for (const auto &path: filesPaths) {
        IloNum cplexSolution = 0;
        auto cplexTimeElapsed = chrono::duration<double>(0);

        const auto data = InputData(path);

        if (shouldPrintSolution && useCPLEX) {
            cout << "Cplex" << " ( " << data.stem << "_" << data.instance << " ): \n" << endl;
        }
        if (useCPLEX || useOnlyCPLEX) {
            const CplexSolver cplexModelSolver(data);

            cplexSolution = cplexModelSolver.getCplexSolution();
            cplexTimeElapsed = cplexModelSolver.getCplexTimeElapsed();

            if (shouldPrintSolution) {
                if (useOnlyCPLEX) {
                    cout << "\n";
                }

                cplexModelSolver.print();
            }
        }

        if (useOnlyCPLEX) {
            return 0;
        }

        optional<vector<int>> bestNEHAlgorithmIndividual;

        if (runNEHAlgorithm) {
            auto [_bestNEHAlgorithmIndividual, fitnessValue] = NEHAlgorithm(data);
            bestNEHAlgorithmIndividual = _bestNEHAlgorithmIndividual;

            if (shouldPrintSolution) {
                cout << "\nNEH Algorithm" << " ( " << data.stem << "_" << data.instance << " ): \n" << endl;

                printNEHAlgorithm(_bestNEHAlgorithmIndividual, fitnessValue, "NEH Best Permutation: ");
            }
        }

        if (shouldPrintSolution) {
            cout << "\nGenetic Algorithm" << " ( " << data.stem << "_" << data.instance << " ): \n" << endl;
        }

        const vector<vector<int>> &processingTime = data.processingTime;
        const vector<int> &deadlines = data.deadlines;

        populationSize += ceil(sqrt(deadlines.size()));

        const GeneticAlgorithmRunner runner = [bestNEHAlgorithmIndividual, processingTime, deadlines](
            size_t maximumIterations, size_t maxIterationsWithoutImprovement, size_t mutationProbability,
            size_t individualTransferRate, size_t populationSize) -> GeneticAlgorithmRunner {
            if (bestNEHAlgorithmIndividual.has_value()) {
                return {maximumIterations, maxIterationsWithoutImprovement, processingTime, deadlines,
                    Population(mutationProbability, individualTransferRate, populationSize, processingTime, deadlines,
                        {
                            Individual(processingTime, deadlines, bestNEHAlgorithmIndividual.value(), false)
                        })
                };
            }

            return {maximumIterations, maxIterationsWithoutImprovement, mutationProbability,
                individualTransferRate, populationSize, processingTime, deadlines};
        }(maximumIterations, maxIterationsWithoutImprovement, mutationProbability, individualTransferRate, populationSize);

        if (shouldPrintSolution) {
            runner.print();
            cout << endl;
        }

        Line line = hasObjectiveValueReference ?
            Line(data.stem, data.instance, data.objectiveValue, cplexSolution, cplexTimeElapsed,
                runner.getObjectiveValue(), runner.getElapsedTime()):
            Line(data.stem, data.instance, cplexSolution, cplexTimeElapsed,
                runner.getObjectiveValue(), runner.getElapsedTime());

        latexTable.addLine(line);
    }

    if (shouldPrintTable) {
        std::ofstream tableFile("../Tables/table_" + currentDateTime() + ".txt");
        tableFile << latexTable;
        tableFile.close();
    }

    return 0;
}