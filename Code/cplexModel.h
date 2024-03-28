#ifndef CPLEXMODEL_H
#define CPLEXMODEL_H

#include "inputData.h"
#include <ilcplex/ilocplex.h>

bool isCplexVerboseActive = false;
bool shouldPrintSolution = false;

int timeLimitInSeconds = 3600;

void cplexModelSolver(const InputData& instanceData);

#endif //CPLEXMODEL_H
