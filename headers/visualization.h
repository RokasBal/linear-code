#ifndef VISUALIZATION_H
#define VISUALIZATION_H

// File used for visualizing various data structures and results

#include "matrix.h"

// Prints given matrix to console
void displayMatrix(const Matrix& matrix);       
// Prints given vector to console
void displayVector(const std::vector<uint8_t>& vector);
// Prints generated syndromes to console
void displaySyndromes(const std::vector<std::pair<int, std::vector<uint8_t>>>& syndromes);
// Compares two vectors and indicates points where they differ
// Example of a print:
// 0 1 1 0 1 1 1 0
// ^ _ _ ^ _ _ _ ^ (^ - indicates a difference, _ - indicates no difference)
void displayVectorWithMistakes(const std::vector<uint8_t>& vec1, const std::vector<uint8_t>& vec2);
// Function for clearing the console, handling different operating systems
void clearConsole();

// Functions used for debugging in development of the program

// Prints differences between two vectors
// Used mostly for debuging due to not working the same across different terminals
void displayVectorDifferences(const std::vector<uint8_t>& vec1, const std::vector<uint8_t>& vec2);
// Prints the number of introduced errors between two vectors
int getIntroducedErrorCount(const std::vector<uint8_t>& vec1, const std::vector<uint8_t>& vec2);

#endif // VISUALIZATION_H
