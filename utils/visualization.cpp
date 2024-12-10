#include "../headers/visualization.h"
#include <iostream>

void displayMatrix(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            std::cout << static_cast<int>(elem) << " ";
        }
        std::cout << std::endl;
    }
}

void displayVector(const std::vector<uint8_t>& vector) {
    if (vector.empty()) {
        return;
    }

    for (const auto& elem : vector) {
        std::cout << static_cast<int>(elem) << " ";
    }
    
    std::cout << std::endl;
}

void displaySyndromes(const std::vector<std::pair<int, std::vector<uint8_t>>>& syndromes) {
    for (const auto& syndromePair : syndromes) {
        int weight = syndromePair.first;
        const std::vector<uint8_t>& syndrome = syndromePair.second;
        std::cout << "Weight: "<<weight <<", Syndrome: ";
        displayVector(syndrome);
    }
}

void displayVectorDifferences(const std::vector<uint8_t>& vec1, const std::vector<uint8_t>& vec2) {
    const std::string RED = "\033[31m";
    const std::string RESET = "\033[0m";

    for (size_t i = 0; i < vec2.size(); ++i) {
        if (i != 0) std::cout << " ";
        
        if (vec1[i] != vec2[i]) {
            std::cout << RED << static_cast<int>(vec2[i]) << RESET;
        } else {
            std::cout << static_cast<int>(vec2[i]);
        }
    }
    std::cout << std::endl;
}
