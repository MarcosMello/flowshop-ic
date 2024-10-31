#include "../GeneticAlgorithm.h"
#include "../cplexModel.h"

Conclusion::Conclusion(const vector<vector<int>> &processingTime, const vector<int> &deadlines, Individual *individual,
                       const bool cplexConclusion) : processingTime(processingTime),
                                                     deadlines(deadlines),
                                                     individual(individual),
                                                     jobs(processingTime.size()),
                                                     machines(processingTime.front().size()),
                                                     objectiveValue(0) {
    if (!cplexConclusion) {
        const vector<vector<int>> conclusion = generateConclusion();

        for (size_t i = 0; i < conclusion.back().size(); i++) {
            objectiveValue += abs(conclusion.back()[i] - deadlines[(*individual)[i]]);
        }

        return;
    }

    const CplexSolver cplexSolver(individual->getValue(),
                                          static_cast<int>(jobs),
                                          static_cast<int>(machines),
                                          processingTime,
                                          deadlines);

    objectiveValue = static_cast<int>(cplexSolver.getCplexSolution());
}

Conclusion::Conclusion(const vector<vector<int>> &processingTime, const vector<int> &deadlines, Individual *individual) :
    Conclusion(processingTime, deadlines, individual, false) {}

[[nodiscard]]
vector<vector<int>> Conclusion::generateConclusion() const {
    vector conclusion(this->machines, vector(this->jobs, 0));

    auto safeDecrement = [](const size_t i) -> size_t {
        return i > 0 ? i - 1 : 0;
    };

    for (size_t i = 0; i < conclusion.size(); i++) {
        for (size_t j = 0; j < conclusion.front().size(); j++) {
            conclusion[i][j] = max(conclusion[safeDecrement(i)][j], conclusion[i][safeDecrement(j)]) +
                this->processingTime[(*individual)[j]][i];
        }
    }

    auto nextTaskStartingTime = [&](const size_t iterator, const size_t machine) -> int {
        return conclusion[machine][iterator + 1] -
            this->processingTime[this->individual->operator[](iterator + 1)][machine];
    };

    conclusion.back().back() = conclusion.back().back() < deadlines[individual->operator[](jobs - 1)] ?
        deadlines[individual->operator[](jobs - 1)] : conclusion.back().back();

    for (size_t i = 2; i <= conclusion.back().size(); i++) {
        const size_t backwardsIterator = conclusion.back().size() - i;

        if (conclusion.back()[backwardsIterator] < deadlines[individual->operator[](backwardsIterator)]) {
            if (nextTaskStartingTime(backwardsIterator, conclusion.size() - 1) >= deadlines[individual->operator[](backwardsIterator)]) {
                conclusion.back()[backwardsIterator] = deadlines[individual->operator[](backwardsIterator)];
            } else {
                size_t k = backwardsIterator, quantityOfEarlyTasks = 1, quantityOfChainedTasks = 1;

                while (k < jobs - 1 &&
                       nextTaskStartingTime(k, conclusion.size() - 1) == conclusion.back()[k]) {
                    if (conclusion.back()[k + 1] < deadlines[individual->operator[](k + 1)]) {
                        quantityOfEarlyTasks++;
                    }

                    k++;
                    quantityOfChainedTasks++;
                }

                if (quantityOfEarlyTasks > quantityOfChainedTasks/2) {
                    for (size_t j = backwardsIterator; j <= k; j++) {
                        conclusion.back()[j]++;
                    }

                    i--;
                }
            }
        }
    }

    return conclusion;
}

[[nodiscard]]
int Conclusion::getObjectiveValue() const {
    return this->objectiveValue;
}
