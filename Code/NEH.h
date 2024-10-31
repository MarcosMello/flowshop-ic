#ifndef NEH_H
#define NEH_H

#include "GeneticAlgorithm.h"

inline bool runNEHAlgorithm = true;
inline bool nehAlgorithmLog = false;

[[nodiscard]]
pair<vector<int>, size_t> NEHAlgorithm(const InputData& instanceData);
void printNEHAlgorithm(const variant<vector<size_t>, vector<int>>& variantBestJobPermutation,
                       size_t fitnessValue, const string& message);

#endif //NEH_H
