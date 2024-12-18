#ifndef GENERATESYNDROMES_H
#define GENERATESYNDROMES_H

// File used for generating syndromes needed for decoding

#include "matrix.h"
#include <vector>
#include <unordered_map>

// Hash function, used to store syndromes in an unordered_set
struct VectorHash {
    std::size_t operator()(const std::vector<uint8_t>& v) const {
        uint64_t ret = 0;
        for (uint8_t i : v) {
            ret |= i;
            ret <<= 1;
        }
        return ret;
    }
};

// Set syndromeTable type for simpler code
using syndromesTable = std::unordered_map<std::vector<uint8_t>, int, VectorHash>;

// Function for generating syndromes
syndromesTable generateSyndromes(int n, int k, const Matrix& H, bool showProgress);

// Function for generating all combinations for a given weight and length
void generateCombinations(int n, int weight, std::vector<std::vector<int>>& combinations);

#endif // GENERATESYNDROMES_H