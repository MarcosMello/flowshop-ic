#ifndef NEH_H
#define NEH_H

#include "GeneticAlgorithm.h"

inline bool runNEHAlgorithm = true;

[[nodiscard]]
Individual NEHAlgorithm(const InputData& instanceData);

#endif //NEH_H
