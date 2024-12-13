#ifndef GENERATESYNDROMES_H
#define GENERATESYNDROMES_H

#include "matrix.h"
#include <vector>

using syndromesTable = std::vector<std::pair<int, std::vector<uint8_t>>>;
syndromesTable generateSyndromes(int n, int k, const Matrix& H);
// void generateCombinations(int n, int weight, int start, std::vector<int>& indices, std::vector<std::vector<int>>& combinations);
void generateCombinations(int n, int weight, std::vector<std::vector<int>>& combinations);
inline int nCr(int n, int r);
struct VectorHash {
    std::size_t operator()(const std::vector<uint8_t>& v) const {
        std::size_t hash = 0;
        for (uint8_t i : v) {
            hash ^= std::hash<uint8_t>{}(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

#endif // GENERATESYNDROMES_H