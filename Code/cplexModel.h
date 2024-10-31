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
    CplexSolver(const vector<int>& individual,
                int jobs,
                int machines,
                const vector<vector<int>>& processingTime,
                const vector<int>& deadlines);
    CplexSolver(const InputData& instanceData, const vector<int>& individual);

    IloCplex modelSolver(IloEnv env);

    [[nodiscard]] double getCplexSolution() const;
    [[nodiscard]] chrono::duration<double> getCplexTimeElapsed() const;

    void print() const;

private:
    vector<int> individual;

    int jobs, machines;
    vector<vector<int>> processingTime;
    vector<int> deadlines;

    variant<vector<vector<int>>, IloArray<IloIntVarArray>> variantPosition;

    double cplexSolution;
    double cplexGap;
    chrono::duration<double> cplexTimeElapsed = chrono::duration<double>(0);
};

void cplexModelSolver(const InputData& instanceData);

#endif //CPLEXMODEL_H
