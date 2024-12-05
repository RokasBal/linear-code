#include "../headers/matrixGen.h"
#include <cstdlib>
#include <ctime> 
#include <iostream>

Matrix generateRandomMatrix(int k, int n) {
    if (k <= 0 || n <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    if (k >= n) {
        throw std::invalid_argument("k must be less than n for generator matrix");
    }

    Matrix matrix(k, std::vector<uint8_t>(n, 0)); 

    std::srand(static_cast<unsigned>(std::time(0)));

    for (int i = 0; i < k; i++) {
        matrix[i][i] = 1;
    }

    // Fill remaining part with random binary values
    for (int i = 0; i < k; i++) {
        for (int j = k; j < n; j++) {
            matrix[i][j] = static_cast<uint8_t>(std::rand() % 2);
        }
    }

    return matrix;
}

Matrix generateParityMatrix(const Matrix& matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        throw std::invalid_argument("Input matrix cannot be empty");
    }

    int k = matrix.size();    // Number of rows in G
    int n = matrix[0].size(); // Number of columns in G

    if (n <= k) {
        throw std::invalid_argument("Number of columns must be greater than rows (n > k)");
    }

    // Initialize empty parity matrix
    Matrix parityMatrix(n - k, std::vector<uint8_t>(n, 0));

    // Transpose data from G to H
    for (int i = 0; i < k; i++) {
        for (int j = k; j < n; j++) {
            parityMatrix[j - k][i] = matrix[i][j];
        }
    }

    // Fill remainder with standard matrix
    for (int i = 0; i < n - k; i++) {
        parityMatrix[i][k + i] = 1;
    }

    return parityMatrix;
}

void displayMatrix(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            std::cout << static_cast<int>(elem) << " ";
        }
        std::cout << std::endl;
    }
}
