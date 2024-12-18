#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "matrix.h"
#include "visualization.h"
#include "calculations.h"
#include "generateMatrix.h"
#include "generateSyndromes.h"
#include "decode.h"
#include "formatToBinary.h"
#include <chrono>
#include <iostream>
#include <tuple>
#include <iomanip>
#include <fstream>
#include <sstream>

// File used to test performance and capabilities of the program and encoding/decoding algorithms

void benchmark(int minN, int maxN, int minK, int maxK, double errorRate) ;

#endif // BENCHMARK_H