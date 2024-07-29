#include "../GeneticAlgorithm.h"

Conclusion::Conclusion(const vector<vector<int>> &processingTime, const vector<int> &deadlines, Individual *individual) :
        processingTime(processingTime), deadlines(deadlines), individual(individual),
        jobs(processingTime.size()), machines(processingTime.front().size()),
        conclusion(generateConclusion()), objectiveValue(0) {
    for (size_t i = 0; i < conclusion.back().size(); i++) {
        objectiveValue += abs(conclusion.back()[i] - deadlines[(*individual)[i]]);
    }
}

[[nodiscard]]
vector<vector<int>> Conclusion::generateConclusion() const {
    vector conclusion(this->machines, vector(this->jobs, 0));

    auto safeDecrement = [](const size_t i) -> size_t {
        return i > 0 ? i - 1 : 0;
    };

    auto getIndexOnConclusion = [&](const vector<int>::reverse_iterator iterator) -> size_t {
        return distance(iterator, conclusion.back().rend()) - 1;
    };

    auto taskStartingTimeOnLastMachine = [&](const vector<int>::reverse_iterator iterator) -> int {
        return *iterator - this->processingTime[this->individual->operator[](getIndexOnConclusion(iterator))].back();
    };

    for (size_t i = 0; i < conclusion.size(); i++) {
        for (size_t j = 0; j < conclusion.front().size(); j++) {
            conclusion[i][j] = max(conclusion[safeDecrement(i)][j], conclusion[i][safeDecrement(j)]) +
                this->processingTime[(*individual)[j]][i];
        }
    }

    conclusion.back().back() = conclusion.back().back() < deadlines[individual->back()] ?
        deadlines[individual->back()] : conclusion.back().back();

    for (auto reverseIterator = conclusion.back().rbegin() + 1; reverseIterator != conclusion.back().rend(); ++reverseIterator) {
        const auto nextIterator = reverseIterator - 1;

        if (*reverseIterator > deadlines[getIndexOnConclusion(reverseIterator)]) {
            continue;
        }

        if (taskStartingTimeOnLastMachine(nextIterator) >= deadlines[getIndexOnConclusion(reverseIterator)]) {
            *reverseIterator = deadlines[getIndexOnConclusion(reverseIterator)];
            continue;
        }

        *reverseIterator = taskStartingTimeOnLastMachine(nextIterator);
    }

    for (const auto conclusionTime : conclusion.back()) {
        cout << conclusionTime << " ";
    }
    cout << endl;

    for (const auto job: individual->getValue()) {
        cout << job + 1 << " ";
    }
    cout << endl;

    return conclusion;
}

[[nodiscard]]
int Conclusion::getObjectiveValue() const {
    return this->objectiveValue;
}
