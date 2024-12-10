#include "../headers/generateSyndromes.h"
#include "../headers/matrixMath.h"
#include "../headers/visualization.h"
#include <cmath>
#include <iostream>

syndromesTable generateSyndromes(int n, int k, const Matrix& H) {
    syndromesTable syndromes;
    int calculatedSyndromes = 0;
    int syndromeCount = pow(2, n - k);

    // Matrix syndromes(syndromeCount, std::vector<uint8_t>(2, 0));
    std::vector<uint8_t> zeroCoset(n, 0);
    Matrix syndromeMatrix = multiplyMatrices(H, transposeVector(zeroCoset));
    std::vector<uint8_t> syndrome = transposeMatrix(syndromeMatrix)[0];
    syndromes.emplace_back(0, syndrome);
    calculatedSyndromes++;

    for (int weight = 1; weight <= n && calculatedSyndromes < syndromeCount; ++weight) {
        std::cout << "Calculating syndromes with weight " << weight << std::endl;
        std::vector<int> positions;
        std::vector<std::vector<int>> combinations;
        
        generateCombinations(n, weight, 0, positions, combinations);
        
        // Create coset for each combination
        for (const auto& combination : combinations) {
            std::vector<uint8_t> coset(n, 0);
            for (int pos : combination) {
                coset[pos] = 1;
            }
            
            // Calculate syndrome
            syndromeMatrix = multiplyMatrices(H, transposeVector(coset));
            syndrome = transposeMatrix(syndromeMatrix)[0];
            
            // Check if syndrome already exists
            bool found = false;
            for (const auto& existingSyndrome : syndromes) {
                if (existingSyndrome.second == syndrome) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                syndromes.emplace_back(weight, syndrome);
                calculatedSyndromes++;
                std::cout << "Calculated " << calculatedSyndromes << " syndromes" << std::endl;
                if (calculatedSyndromes == syndromeCount) {
                    return syndromes;
                }
            }
        }
    }

    return syndromes;
}

void generateCombinations(int n, int weight, int start, std::vector<int>& indices, std::vector<std::vector<int>>& combinations) {
    if (indices.size() == weight) {
        combinations.push_back(indices);
        return;
    }

    for (int i = n - 1 - start; i >= 0; --i) {
        indices.push_back(i);
        generateCombinations(n, weight, n - i, indices, combinations);
        indices.pop_back();
    }
}