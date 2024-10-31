#include "../GeneticAlgorithm.h"

Population::Population(const size_t mutationProbability, const size_t individualTransferRate,
    const size_t populationSize, const vector<vector<int>> &processingTime,
    const vector<int> &deadlines) : generation(1), mutationProbability(mutationProbability),
    individualTransferRate((individualTransferRate * populationSize)/100), populationSize(populationSize),
    processingTime(processingTime), deadlines(deadlines) {
    this->population.reserve(this->populationSize);

    generate_n(back_inserter(this->population),
                    populationSize,
                [processingTime, deadlines](){ return Individual(processingTime, deadlines); });

    this->sort();
}

Population::Population(const size_t mutationProbability, const size_t individualTransferRate,
    const size_t populationSize, const vector<vector<int>> &processingTime, const vector<int> &deadlines,
    const vector<Individual> &populationIndividualVector) : generation(1), mutationProbability(mutationProbability),
    individualTransferRate((individualTransferRate * populationSize)/100), populationSize(populationSize),
    processingTime(processingTime), deadlines(deadlines){
    for (const auto & i : populationIndividualVector) {
        this->population.emplace_back(i);
    }

    generate_n(back_inserter(this->population),
               populationSize - populationIndividualVector.size(),
               [processingTime, deadlines](){ return Individual(processingTime, deadlines); });

    this->sort();
}

void Population::generateNextGeneration() {
    this->generation++;

    size_t localPopulationSize = this->populationSize;

    vector<Individual> newPopulation;
    newPopulation.reserve(localPopulationSize);

    size_t counter = 0;
    vector<double> probabilities(localPopulationSize);
    generate(probabilities.begin(), probabilities.end(),
        [this, localPopulationSize, counter]() -> double {
            return static_cast<double>(2 * (localPopulationSize - counter)) /
                static_cast<double>(this->population.size() * (this->population.size() + 1));
        }
    );

    discrete_distribution const distribution{cbegin(probabilities), cend(probabilities)};

    for (size_t i = 0; i < this->populationSize; i++) {
        if (i < this->individualTransferRate) {
            newPopulation.push_back(this->population[i]);

            continue;
        }

        vector<decltype(distribution)::result_type> selectedParents(NUMBER_OF_PARENTS);

        generate_n(back_inserter(selectedParents), NUMBER_OF_PARENTS,
            [distribution = distribution, generator = defaultRandomEngine]() mutable {
                return distribution(generator);
            }
        );

        const vector<int> childVector = population[selectedParents.front()]
            .createChildValueVector(population[selectedParents.back()]);
        auto child = Individual(this->processingTime, this->deadlines, childVector, false);

        if (getRandomNumber(0, 100) < this->mutationProbability) {
            child.mutate();
        }

        newPopulation.push_back(child);
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

void Population::changeLeastFittest(const Individual &individual) {
    this->population.back() = individual;

    this->sort();
}

void Population::sort(){
    std::sort(this->population.begin(),
        this->population.end(),
        [](const auto &left, const auto &right){return left < right;}
    );
}
