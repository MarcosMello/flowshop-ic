#include "../NEH.h"

class Job {
public:
    Job(const size_t id, const int deadline, const vector<int>& processingTime) : id(id), deadline(deadline),
        totalProcessingTime(accumulate(processingTime.begin(), processingTime.end(), 0)) {}

    [[nodiscard]]
    size_t getId() const {
        return this->id;
    }

    [[nodiscard]]
    string toString() const {
        return to_string(this->id + 1) + " - Total Processing Time: " + to_string(totalProcessingTime) + " - Deadline: " + to_string(deadline);
    }

    [[nodiscard]]
    bool operator<(const Job& other) const{
        if (this->deadline == other.deadline) {
            if (this->totalProcessingTime == other.totalProcessingTime) {
                return this->id < other.id;
            }

            return this->totalProcessingTime > other.totalProcessingTime;
        }

        return this->deadline < other.deadline;
    }

private:
    size_t id;

    int deadline;
    int totalProcessingTime;
};

[[nodiscard]]
vector<vector<size_t>> getJobPermutations(const vector<size_t>& orderedJobs, const size_t jobToBeAddedID) {
    vector<vector<size_t>> jobPermutations;

    for (size_t i = 0; i < orderedJobs.size() + 1; i++) {
        vector newPermutation(orderedJobs);
        newPermutation.insert(newPermutation.begin() + i, jobToBeAddedID);

        jobPermutations.emplace_back(newPermutation);
    }

    return jobPermutations;
}

[[nodiscard]]
Individual makeIndividualWithJobPermutation(const vector<size_t>& jobPermutation, const InputData& instanceData) {
    vector<vector<int>> processingTime(jobPermutation.size());
    vector<int> deadlines(jobPermutation.size());

    for (size_t i = 0; i < jobPermutation.size(); i++) {
        processingTime[i] = instanceData.processingTime[jobPermutation[i]];
        deadlines[i] = instanceData.deadlines[jobPermutation[i]];
    }

    vector<int> value(jobPermutation.size());
    iota(begin(value), end(value), 0);

    return {processingTime, deadlines, value, false};
}

[[nodiscard]]
pair<vector<int>, size_t> NEHAlgorithm(const InputData& instanceData) {
    vector<Job> jobs;

    for (size_t i = 0; i < static_cast<size_t>(instanceData.jobs); i++) {
        jobs.emplace_back(i, instanceData.deadlines[i], instanceData.processingTime[i]);
    }

    sort(jobs.begin(), jobs.end());

    if (nehAlgorithmLog) {
        for (const Job& job : jobs) {
            cout << job.toString() << endl;
        }
    }

    auto jobIterator = jobs.begin();

    vector ordered {jobIterator->getId()};
    ++jobIterator;

    size_t bestIndividualValue = 0;

    while (jobIterator != jobs.end()) {
        Job jobToBeAdded = *(jobIterator);

        vector jobPermutations {getJobPermutations(ordered, jobToBeAdded.getId())};

        if (nehAlgorithmLog) {
            cout << endl;
            for (size_t i = 0; i < jobPermutations.size(); i++) {
                auto jobPermutation = jobPermutations[i];

                for (size_t j = 0; j < jobPermutation.size(); j++) {
                    cout << jobPermutation[j] + 1
                         << " ," [(j + 1) == jobPermutation.size() && (i + 1) != jobPermutations.size()];
                }

                cout << " ";
            }
            cout << endl;
        }

        vector bestJobPermutation = jobPermutations.front();
        Individual bestIndividual = makeIndividualWithJobPermutation(bestJobPermutation, instanceData);

        for (const auto& jobPermutation : jobPermutations) {
            Individual actualIndividual = makeIndividualWithJobPermutation(jobPermutation, instanceData);

            if (nehAlgorithmLog) {
                cout << "This Fitness Value: " << actualIndividual.getFitness()
                     << " - Best Fitness Value: " << bestIndividual.getFitness() << endl;
            }

            if (actualIndividual.getFitness() < bestIndividual.getFitness()) {
                bestIndividual = actualIndividual;
                bestIndividualValue = bestIndividual.getFitness();
                bestJobPermutation = jobPermutation;
            }
        }

        ordered = bestJobPermutation;

        if (nehAlgorithmLog) {
            printNEHAlgorithm(bestJobPermutation, bestIndividualValue, "This Round Best Job Permutation: ");
        }

        ++jobIterator;
    }

    vector finalOrderedJobs = [ordered]() -> vector<int> {
        vector<int> _finalOrderedJobs(ordered.size());

        for (size_t i = 0; i < ordered.size(); i++) {
            _finalOrderedJobs[i] = static_cast<int>(ordered[i]);
        }

        return _finalOrderedJobs;
    }();

    return {finalOrderedJobs, bestIndividualValue};
}

void printNEHAlgorithm(const variant<vector<size_t>, vector<int>>& variantBestJobPermutation,
                       size_t fitnessValue, const string& message) {
    visit([message, fitnessValue](auto&& bestJobPermutation) {
        cout << message;

        for (const auto job : bestJobPermutation) {
            cout << job + 1 << " ";
        }

        cout << "- Fitness Value: " << fitnessValue << endl;
    }, variantBestJobPermutation);
}
