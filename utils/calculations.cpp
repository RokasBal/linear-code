#include "../headers/calculations.h"
#include <iostream>
#include <random>

Vec encodeMessage(const Matrix& G, const Vec& message, int k, int* addedBits) {
    // size_t messageSize = message.size();
    // size_t partCount = (messageSize + k - 1) / k;
    // std::vector<uint8_t> encodedMessage;

    // for (size_t i = 0; i < partCount; i++) {
    //     std::vector<uint8_t> part;
    //     size_t partStart = i * k;
    //     size_t partEnd = std::min(partStart + k, messageSize);
        
    //     part.assign(message.begin() + partStart, message.begin() + partEnd);

    //     while (part.size() < k) {
    //         part.push_back(0);
    //     }

    //     std::vector<uint8_t> encodedPart = multiplyMatrixVector(G, part);
        
    //     encodedMessage.insert(encodedMessage.end(), encodedPart.begin(), encodedPart.end());
    // }

    Vec part = message;

    while (part.size() < k) {
        part.push_back(0);
        *addedBits += 1;
    }

    Vec encodedPart = multiplyMatrixVector(G, part);

    return encodedPart;
}

Vec multiplyMatrixVector(const Matrix& matrix, const Vec& vector) {
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();

    if (rows == 0 || cols == 0 || vector.size() != rows) {
        throw std::invalid_argument("Invalid dimensions for matrix-vector multiplication");
    }

    Vec result(cols, 0);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[j] ^= (vector[i] & matrix[i][j]);
        }
    }

    return result;
}

Matrix multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2) {
    size_t rows = matrix1.size();
    size_t cols = matrix2[0].size();
    size_t common = matrix2.size();

    if (rows == 0 || cols == 0 || common != matrix1[0].size()) {
        throw std::invalid_argument("Invalid dimensions for matrix-matrix multiplication");
    }

    Matrix result(rows, std::vector<uint8_t>(cols, 0));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t k = 0; k < common; ++k) {
            uint8_t multiplier = matrix1[i][k];
            if (multiplier == 0) continue; 
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] ^= (multiplier & matrix2[k][j]);
            }
        }
    }

    return result;
}

std::vector<uint8_t> introduceErrors(const Vec& vector, double errorRate) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Create copy of input vector
    Vec result = vector;

    for (size_t i = 0; i < result.size(); i++) {
        if (dis(gen) < errorRate) {
            result[i] ^= 1; // Flip bit using XOR with 1
        }
    }

    return result;
}

Matrix transposeMatrix(const Matrix& matrix) {
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();
    Matrix transposed(cols, std::vector<uint8_t>(rows));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    return transposed;
}

Matrix transposeVector(const Vec& vector) {
    Matrix transposed(vector.size(), Vec(1));
    for (size_t i = 0; i < vector.size(); ++i) {
        transposed[i][0] = vector[i];
    }
    return transposed;
}