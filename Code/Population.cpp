#include "GeneticAlgorithm.h"

Population::Population(const size_t mutationProbability, const size_t individualTransferRate,
    const size_t populationSize, const vector<vector<int>> &processingTime,
    const vector<int> &deadlines) : generation(1), mutationProbability(mutationProbability),
    individualTransferRate((individualTransferRate * populationSize)/100), populationSize(populationSize),
    processingTime(processingTime), deadlines(deadlines) {
    this->population.reserve(this->populationSize);

    generate_n(back_inserter(this->population),
                    populationSize,
                    [processingTime, deadlines](){return Individual(processingTime, deadlines); });

    this->sort();
}

Population::Population(const size_t mutationProbability, const size_t individualTransferRate,
    const vector<vector<int>> &processingTime, const vector<int> &deadlines,
    const vector<Individual> &population) : generation(1), mutationProbability(mutationProbability),
    individualTransferRate((individualTransferRate * population.size())/100), populationSize(population.size()), population(population),
    processingTime(processingTime), deadlines(deadlines){
    this->sort();
}

void Population::generateNextGeneration() {
    this->generation++;

    vector<Individual> newPopulation;
    newPopulation.reserve(this->populationSize);

    size_t counter = 0;
    size_t individualSize = this->population.front().size();
    vector<double> probabilities(individualSize);
    generate(probabilities.begin(), probabilities.end(),
        [this, individualSize, counter]() -> double {
            return static_cast<double>(2 * (individualSize - counter)) /
                static_cast<double>(this->population.size() * (this->population.size() + 1));
        }
    );

    discrete_distribution const distribution{cbegin(probabilities), cend(probabilities)};

    for (size_t i = 0; i < this->populationSize; i++) {
        if (i < this->individualTransferRate) {
            newPopulation[i] = this->population[i];

            continue;
        }

        vector<decltype(distribution)::result_type> selectedParents(NUMBER_OF_PARENTS);

        generate_n(back_inserter(selectedParents), NUMBER_OF_PARENTS,
            [distribution = distribution, generator = default_random_engine(random_device()())]() mutable {
                return distribution(generator);
            }
        );

        vector<int> childVector = population[selectedParents.front()]
            .createChildValueVector(population[selectedParents.back()]);
        auto child = Individual(this->processingTime, this->deadlines, childVector);

        if (getRandomNumber(0, 100) < this->mutationProbability) {
            child.mutate();
        }

        newPopulation[i] = child;
    }

    this->population = std::move(newPopulation);
    this->sort();
}

[[nodiscard]]
const Individual &Population::front() const {
    return population.front();
}

[[nodiscard]]
size_t Population::getObjectiveValue() const {
    return this->front().getFitness();
}

[[nodiscard]]
vector<int> Population::getObjectiveJobOrder() const{
    return this->front().getValue();
}

void Population::sort(){
    std::sort(this->population.begin(),
        this->population.end(),
        [](const auto &left, const auto &right){return left < right;}
    );
}
