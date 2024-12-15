#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "matrix.h"
#include "visualization.h"
#include "calculations.h"
#include "generateMatrix.h"
#include "generateSyndromes.h"
#include "decode.h"
#include <chrono>
#include <iostream>
#include <tuple>
#include <iomanip>

void benchmark(int minN, int maxN, int minK, int maxK, double errorRate) ;

#endif // BENCHMARK_H