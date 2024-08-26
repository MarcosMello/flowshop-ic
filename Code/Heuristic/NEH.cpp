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

    return {processingTime, deadlines, value, true};
}

[[nodiscard]]
Individual NEHAlgorithm(const InputData& instanceData) {
    vector<Job> jobs;

    for (size_t i = 0; i < instanceData.jobs; i++) {
        jobs.emplace_back(i, instanceData.deadlines[i], instanceData.processingTime[i]);
    }

    sort(jobs.begin(), jobs.end());

    for (const Job& job : jobs) {
        cout << job.toString() << endl;
    }
    cout << endl;

    auto jobIterator = jobs.begin();

    vector ordered {jobIterator->getId()};
    ++jobIterator;

    while (jobIterator != jobs.end()) {
        Job jobToBeAdded = *(jobIterator);

        vector jobPermutations {getJobPermutations(ordered, jobToBeAdded.getId())};

        for (const auto& jobPermutation : jobPermutations) {
            for (const auto job : jobPermutation) {
                cout << job + 1 << " ";
            }
            cout << ", ";
        }
        cout << endl;

        vector bestJobPermutation = jobPermutations.front();
        Individual bestIndividual = makeIndividualWithJobPermutation(bestJobPermutation, instanceData);

        for (const auto& jobPermutation : jobPermutations) {
            Individual actualIndividual = makeIndividualWithJobPermutation(jobPermutation, instanceData);

            cout << "atual: " << actualIndividual.getFitness() << "- best: " << bestIndividual.getFitness() << endl;

            if (actualIndividual.getFitness() < bestIndividual.getFitness()) {
                bestIndividual = actualIndividual;
                bestJobPermutation = jobPermutation;
            }
        }

        ordered = bestJobPermutation;

        cout << "Best Job Permutation: ";
        for (const auto job : bestJobPermutation) {
            cout << job + 1 << " ";
        }
        cout << endl;

        ++jobIterator;
    }

    for (const auto jobID : ordered) {
        cout << jobID + 1 << " ";
    }
    cout << endl;

    const Individual bestIndividual = makeIndividualWithJobPermutation(ordered, instanceData);
    cout << bestIndividual.getFitness() << endl;

    return bestIndividual;
}
