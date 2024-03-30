#ifndef FLOWSHOP_IC_INPUTDATA_H
#define FLOWSHOP_IC_INPUTDATA_H

#include "bits/stdc++.h"

using namespace std;

inline bool hasObjectiveValueReference = false;

class InputData{
public:
    std::string stem;
    std::string instance;
    int objectiveValue = 0;

    int jobs;
    int machines;
    std::vector< std::vector<int> > processingTime;
    std::vector<int> deadlines;

    explicit InputData(const std::string& path);
};

#endif //FLOWSHOP_IC_INPUTDATA_H
