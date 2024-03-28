#include "../GeneticAlgorithm.h"

Conclusion::Conclusion(const vector<vector<int>> &processingTime, const vector<int> &deadlines, Individual *individual) :
        processingTime(processingTime), deadlines(deadlines), individual(individual),
        jobs(processingTime.size()), machines(processingTime.front().size()),
        conclusion(generateConclusion()), objectiveValue(0) {
    for (size_t i = 0; i < deadlines.size(); i++) {
        objectiveValue += abs(conclusion[i][conclusion[i].size() - 1] - deadlines[(*individual)[i]]);
    }
}

[[nodiscard]]
bool Conclusion::isJobEarly(const size_t i) const {
    return this->conclusion[i].back() < this->deadlines[(*this->individual)[i]];
};

[[nodiscard]]
int Conclusion::getLastTaskStartingTime(const size_t i) const {
    return this->conclusion[i].back() - this->processingTime[(*this->individual)[i]].back();
};

[[nodiscard]]
bool Conclusion::isNextJobProcessedAfterJobDeadline(const size_t i) const {
    return this->getLastTaskStartingTime(i + 1) >= this->deadlines[(*this->individual)[i]];
};

[[nodiscard]]
bool Conclusion::isNextJobDependentOnJobConclusion(const size_t i) const {
    return this->getLastTaskStartingTime(i + 1) == this->conclusion[i].back();
};

[[nodiscard]]
size_t Conclusion::nextJobProcessedBeforeJobDeadline(const size_t outerI) {
    size_t innerI = outerI;
    size_t earlyJobs = 0, chainedJobsByConclusionTime = 0;

    while (innerI < this->jobs - 1 && this->isNextJobDependentOnJobConclusion(innerI)) {
        if (this->isJobEarly(innerI + 1)) {
            earlyJobs++;
        }

        innerI++;
        chainedJobsByConclusionTime++;
    }

    if (earlyJobs > chainedJobsByConclusionTime/2) {
        for (size_t i = outerI; i <= innerI; i++) {
            this->conclusion[i].back()++;
        }

        return 1;
    }

    return 0;
};

[[nodiscard]]
size_t Conclusion::earlyTaskVerificationFunction(const size_t i) {
    if (!this->isJobEarly(i)) {
        return 0;
    }

    if (this->conclusion[i] == this->conclusion.back()) {
        this->conclusion.back().back() = this->deadlines[this->individual->back()];

        return 0;
    }

    if (this->isNextJobProcessedAfterJobDeadline(i)) {
        this->conclusion[i].back() = this->deadlines[(*this->individual)[i]];

        return 0;
    }

    return this->nextJobProcessedBeforeJobDeadline(i);
};

[[nodiscard]]
vector<vector<int>> Conclusion::generateConclusion() {
    vector conclusion(this->jobs, vector(this->machines, 0));

    auto safeDecrement = [](const size_t i) -> size_t {
        return i > 0 ? i - 1 : 0;
    };

    for (size_t i = 0; i < this->jobs; i++) {
        for (size_t j = 0; j < this->machines; j++) {
            conclusion[i][j] =
                max(conclusion[safeDecrement(i)][j], conclusion[i][safeDecrement(j)]) +
                    this->processingTime[(*this->individual)[i]][j];
        }
    }

    for (size_t i = jobs - 1; i > 0; i--) {
        i += this->earlyTaskVerificationFunction(i);
    }

    size_t counter = 1;

    while (counter) {
        counter = this->earlyTaskVerificationFunction(0);
    }

    return conclusion;
}

[[nodiscard]]
int Conclusion::getObjectiveValue() const {
    return this->objectiveValue;
}
