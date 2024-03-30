#include "../inputData.h"

InputData::InputData(const std::string &path) {
    std::ifstream file(path, std::ios::in);
    if (!file){
        std::cerr << "No such file on: " << path << std::endl;
        throw std::runtime_error("File not found");
    }

    string stemCompleteString = filesystem::path(path).stem().generic_string();

    this->stem = stemCompleteString.substr(0, stemCompleteString.find('_'));
    this->instance = stemCompleteString.substr(stemCompleteString.find('_') + 1);;

    int jobs, machines;
    file >> jobs >> machines;

    this->jobs = jobs;
    this->machines = machines;

    this->processingTime.resize(jobs);
    this->deadlines.resize(jobs);

    for (int i = 0; i < this->jobs; i++){
        this->processingTime[i].resize(this->machines);
        for (int j = 0; j < this->machines; j++){
            file >> this->processingTime[i][j];
        }
    }

    for (int i = 0; i < this->jobs; i++){
        file >> this->deadlines[i];
    }

    if (hasObjectiveValueReference) {
        file >> objectiveValue;
    }
}
