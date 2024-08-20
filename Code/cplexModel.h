#ifndef CPLEXMODEL_H
#define CPLEXMODEL_H

#include "inputData.h"
#include <ilcplex/ilocplex.h>

inline bool isCplexVerboseActive = false;
inline bool shouldPrintSolution = false;

inline int timeLimitInSeconds = 1800;
inline int threadLimit = 1;

class CplexSolver {
public:
    explicit CplexSolver(const InputData& instanceData);
    CplexSolver(const InputData& instanceData, const vector<int>& individual);

    IloCplex modelSolver(IloEnv env);

    void print() const;

private:
    const InputData instanceData;
    vector<int> individual;

    int jobs, machines;
    vector<vector<int>> processingTime;
    vector<int> deadlines;

    variant<vector<vector<int>>, IloArray<IloIntVarArray>> variantPosition;

    double cplexSolution;
    chrono::duration<double> cplexTimeElapsed = chrono::duration<double>(0);
};

void cplexModelSolver(const InputData& instanceData);

#endif //CPLEXMODEL_H
