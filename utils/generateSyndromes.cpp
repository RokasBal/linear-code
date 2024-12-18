#include "../headers/generateSyndromes.h"
#include "../headers/calculations.h"
#include "../headers/visualization.h"
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <algorithm>

syndromesTable generateSyndromes(int n, int k, const Matrix& H, bool showProgress) {
    syndromesTable syndromes; // Vector for storing generated syndromes for returning
    std::unordered_map<std::vector<uint8_t>, int, VectorHash> syndromeMap; // Map for storing unique syndromes
    int syndromeCount = pow(2, n - k); // Calculate the amount of syndromes to generate
    int calculatedSyndromes = 0;

    int progressUpdateInterval = 5;
    int lastProgress = 0;

    // Check for integer overflow
    if (syndromeCount < 0) {
        throw std::invalid_argument("Integer overflow occurred while calculating syndrome count");
    }

    // Get syndrome for zero coset
    Vec zeroCoset(n, 0);
    Matrix syndromeMatrix;
    Matrix cosetTransposed;
    transposeVector(zeroCoset, cosetTransposed);
    multiplyMatrices(H, cosetTransposed, syndromeMatrix);
    Vec syndrome;
    transposeMatrixToVector(syndromeMatrix, syndrome);
    syndromes.emplace_back(0, syndrome);
    syndromeMap[syndrome] = 0;
    calculatedSyndromes++;

    // Generate syndromes with weight from 1 to n
    for (int weight = 1; weight <= n; ++weight) {
        // Break if all syndromes are generated
        if (calculatedSyndromes >= syndromeCount) {
            break;
        }

        std::vector<std::vector<int>> combinations;
        generateCombinations(n, weight, combinations);

        for (const auto& combination : combinations) {
            if (calculatedSyndromes >= syndromeCount) {
                break;
            }

            // Generate coset
            std::vector<uint8_t> coset(n, 0);
            for (int pos : combination) {
                coset[pos] = 1;
            }

            // Calculate syndrome
            Matrix cosetT;
            transposeVector(coset, cosetT);
            multiplyMatrices(H, cosetT, syndromeMatrix);
            transposeMatrixToVector(syndromeMatrix, syndrome);

            // Check uniqueness
            if (syndromeMap.find(syndrome) == syndromeMap.end()) {
                syndromes.emplace_back(weight, syndrome);
                syndromeMap[syndrome] = weight;
                calculatedSyndromes++;

                if (showProgress) {
                    int progress = static_cast<int>(calculatedSyndromes * 100 / syndromeCount);
                    if (progress >= lastProgress + progressUpdateInterval) {
                        printf("Progresas: %d%%\r", progress);
                        fflush(stdout);
                        lastProgress = progress;
                    }
                }
            }
        }
    }

    return syndromes;
}

void generateCombinations(int n, int weight, std::vector<std::vector<int>>& combinations) {
    std::vector<int> positions(n, 0);

    // Set the first weight positions to 1
    for (int i = 0; i < weight; ++i) {
        positions[i] = 1;
    }

    // Generate all combinations of positions using std::prev_permutation
    do {
        std::vector<int> combination;
        // Go trough the positions vector and collect locations where value is 1
        for (int i = 0; i < n; ++i) {
            if (positions[i] == 1) {
                combination.push_back(i);
            }
        }
        // Add the generated combination to the list of combinations
        combinations.push_back(combination);
    // Continue generating possible combinations until all are generated
    } while (std::prev_permutation(positions.begin(), positions.end()));
}