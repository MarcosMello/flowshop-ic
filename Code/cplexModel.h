#ifndef CPLEXMODEL_H
#define CPLEXMODEL_H

#include "inputData.h"
#include <ilcplex/ilocplex.h>

inline bool isCplexVerboseActive = false;
inline bool shouldPrintSolution = false;

inline int timeLimitInSeconds = 3600;
inline int threadLimit = 1;

inline IloNum cplexSolution;
inline chrono::duration<double> cplexTimeElapsed;

void cplexModelSolver(const InputData& instanceData);

#endif //CPLEXMODEL_H
