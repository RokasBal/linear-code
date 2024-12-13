#include "../headers/generateMatrix.h"
#include <cstdlib>
#include <ctime> 
#include <iostream>
#include <sstream>

Matrix generateRandomMatrix(int k, int n) {
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
    int k = matrix.size();    // Number of rows in G
    int n = matrix[0].size(); // Number of columns in G

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

Matrix generateUserMatrix(int k, int n) {
    Matrix matrix(k, std::vector<uint8_t>(n, 0)); 

    for (int i = 0; i < k; ++i) {
        matrix[i][i] = 1;
    }

    if (k == n) return matrix;

    std::cout << "Įveskite matricos dalį A: " << std::endl;
    for (int i = 0; i < k; i++) {
        while (true) {
            std::cout << "Įveskite eilutę " << i + 1 << " (ilgis " << (n - k) << "): ";
            std::string input;
            std::getline(std::cin >> std::ws, input);
            std::istringstream iss(input);
            std::vector<int> values;
            int value;
            bool validInput = true;

            while (iss >> value) {
                if (value != 0 && value != 1) {
                    std::cout << "Netinkama įvestis. Tik '0' ir '1' yra galimi simboliai. Bandykite eilutę įvesti dar kartą." << std::endl;
                    validInput = false;
                    break;
                }
                values.push_back(value);
            }

            if (validInput && values.size() == (n - k)) {
                for (int j = 0; j < n - k; j++) {
                    matrix[i][k + j] = static_cast<uint8_t>(values[j]);
                }
                break;
            } else if (validInput) {
                std::cout << "Netinkama įvestis. Įveskite " << (n - k) << " simbolius." << std::endl;
            }
        }
    }

    return matrix;
}
