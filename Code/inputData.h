#ifndef FLOWSHOP_IC_INPUTDATA_H
#define FLOWSHOP_IC_INPUTDATA_H

#include "bits/stdc++.h"

class InputData{
public:
    int jobs;
    int machines;
    std::vector< std::vector<int> > processingTime;
    std::vector<int> deadlines;

    explicit InputData(const std::string& filename);
};

#endif //FLOWSHOP_IC_INPUTDATA_H
