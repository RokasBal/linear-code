#include "../headers/calculations.h"
#include <iostream>
#include <random>

// Encodes the message or it's part using the generative matrix
Vec encodeMessage(const Matrix& G, const Vec& message, int k, int* addedBits) {
    Vec part = message;

    // If part does not have enough bits, add 0 bits to the end
    // Also keep track of the added bits for removal post decode
    while (part.size() < k) {
        part.push_back(0);
        *addedBits += 1;
    }

    // Encode the part
    Vec encodedPart;
    multiplyMatrixVector(G, part, encodedPart);

    return encodedPart;
}

// Functions used for multiplying matrices and vectors

void multiplyMatrixVector(const Matrix& matrix, const Vec& vector, Vec& result) {
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();

    // Check dimensions
    if (rows == 0 || cols == 0 || vector.size() != rows) {
        throw std::invalid_argument("Invalid dimensions for matrix-vector multiplication");
    }

    // Initialize the result vector to the correct size
    result.resize(cols, 0);

    // Multiply the matrix and vector
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[j] ^= (vector[i] & matrix[i][j]);
        }
    }
}

void multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2, Matrix& result) {
    size_t rows = matrix1.size();
    size_t cols = matrix2[0].size();
    size_t common = matrix2.size();

    // Initialize the result matrix to the correct size with all elements set to 0
    result.assign(rows, std::vector<uint8_t>(cols, 0));

    // Multiply the matrices
    for (size_t i = 0; i < rows; ++i) {
        for (size_t k = 0; k < common; ++k) {
            if (matrix1[i][k] == 0) continue;
            uint8_t multiplier = matrix1[i][k];
            // if (multiplier == 0) continue; 
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] ^= (multiplier & matrix2[k][j]);
            }
        }
    }
}

std::vector<uint8_t> introduceErrors(const Vec& vector, double errorRate) {
    // Random number generator for introducing errors
    std::random_device rd;  // Gets random seed
    std::mt19937 gen(rd()); // Mersenne Twister 19937 generator
    std::uniform_real_distribution<> dis(0.0, 1.0); // Uniform distribution between 0 and 1

    // Create copy of input vector
    Vec result = vector;

    for (size_t i = 0; i < result.size(); i++) {
        if (dis(gen) < errorRate) {
            result[i] ^= 1; // Flip bit using XOR with 1
        }
    }

    return result;
}

void transposeMatrix(const Matrix& matrix, Matrix& result) {
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();
    
    result.assign(cols, std::vector<uint8_t>(rows));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[j][i] = matrix[i][j];
        }
    }
}

void transposeVector(const Vec& vector, Matrix& result) {
    result.assign(vector.size(), Vec(1));
    for (size_t i = 0; i < vector.size(); ++i) {
        result[i][0] = vector[i];
    }
}

void transposeMatrixToVector(const Matrix& matrix, Vec& result) {
    size_t rows = matrix.size();
    result.resize(rows);
    
    for (size_t i = 0; i < rows; ++i) {
        result[i] = matrix[i][0];
    }
}