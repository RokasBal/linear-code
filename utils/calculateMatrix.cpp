#include "../headers/matrixMath.h"
#include <iostream>
#include <random>

std::vector<uint8_t> encodeMessage(const Matrix& G, const std::vector<uint8_t>& message, int k) {
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

    std::vector<uint8_t> part = message;

    while (part.size() < k) {
        part.push_back(0);
    }

    std::vector<uint8_t> encodedPart = multiplyMatrixVector(G, part);

    return encodedPart;
}

std::vector<uint8_t> multiplyMatrixVector(const Matrix& matrix, const std::vector<uint8_t>& vector) {
    if (matrix.empty() || matrix[0].empty() || vector.size() != matrix.size()) {
        throw std::invalid_argument("Invalid dimensions for matrix-vector multiplication");
    }

    std::vector<uint8_t> result(matrix[0].size(), 0);

    for (size_t j = 0; j < matrix[0].size(); j++) {
        for (size_t i = 0; i < vector.size(); i++) {
            result[j] ^= (vector[i] & matrix[i][j]);
        }
    }

    return result;
}

Matrix multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2) {
    size_t rows = matrix1.size();
    size_t cols = matrix2[0].size();
    size_t common = matrix2.size();

    Matrix result(rows, std::vector<uint8_t>(cols, 0));

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            for (size_t k = 0; k < common; k++) {
                result[i][j] ^= (matrix1[i][k] & matrix2[k][j]);
            }
        }
    }

    return result;
}

std::vector<uint8_t> introduceErrors(const std::vector<uint8_t>& vector, double errorRate) {
    if (errorRate < 0 || errorRate >= 1) {
        throw std::invalid_argument("Error rate must be between 0 and 1");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Create copy of input vector
    std::vector<uint8_t> result = vector;

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

Matrix transposeVector(const std::vector<uint8_t>& vector) {
    Matrix transposed(vector.size(), std::vector<uint8_t>(1));
    for (size_t i = 0; i < vector.size(); ++i) {
        transposed[i][0] = vector[i];
    }
    return transposed;
}