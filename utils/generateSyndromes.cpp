#include "../headers/generateSyndromes.h"
#include "../headers/calculations.h"
#include "../headers/visualization.h"
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <algorithm>

syndromesTable generateSyndromes(int n, int k, const Matrix& H, bool showProgress) {
    syndromesTable syndromes;
    std::unordered_set<std::vector<uint8_t>, VectorHash> syndromeSet;
    int calculatedSyndromes = 0;
    int syndromeCount = pow(2, n - k);

    int progressUpdateInterval = 5;
    int lastProgress = 0;

    if (syndromeCount < 0) {
        throw std::invalid_argument("Integer overflow occurred while calculating syndrome count");
    }

    // Syndrome for zero vector
    std::vector<uint8_t> zeroCoset(n, 0);
    Matrix syndromeMatrix = multiplyMatrices(H, transposeVector(zeroCoset));
    std::vector<uint8_t> syndrome = transposeMatrix(syndromeMatrix)[0];
    syndromes.emplace_back(0, syndrome);
    syndromeSet.insert(syndrome);
    calculatedSyndromes++;

    for (int weight = 1; weight <= n; ++weight) {
        if (calculatedSyndromes >= syndromeCount) {
            break;
        }

        // std::cout << "Calculating syndromes with weight " << weight << std::endl;
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
            syndromeMatrix = multiplyMatrices(H, transposeVector(coset));
            syndrome = transposeMatrix(syndromeMatrix)[0];

            // Check uniqueness
            if (syndromeSet.find(syndrome) == syndromeSet.end()) {
                syndromes.emplace_back(weight, syndrome);
                syndromeSet.insert(syndrome);
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
    for (int i = 0; i < weight; ++i) {
        positions[i] = 1;
    }
    do {
        std::vector<int> combination;
        for (int i = 0; i < n; ++i) {
            if (positions[i] == 1) {
                combination.push_back(i);
            }
        }
        combinations.push_back(combination);
    } while (std::prev_permutation(positions.begin(), positions.end()));
}