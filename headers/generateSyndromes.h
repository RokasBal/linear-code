#ifndef GENERATESYNDROMES_H
#define GENERATESYNDROMES_H

#include "matrix.h"
#include <vector>

using syndromesTable = std::vector<std::pair<int, std::vector<uint8_t>>>;
syndromesTable generateSyndromes(int n, int k, const Matrix& H);
void generateCombinations(int n, int weight, int start, std::vector<int>& indices, std::vector<std::vector<int>>& combinations);

#endif // GENERATESYNDROMES_H