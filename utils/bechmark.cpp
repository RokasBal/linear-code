#include "../headers/benchmark.h"

void benchmark(int minN, int maxN, int minK, int maxK, double errorRate) {
    int bits = 0;
    std::vector<std::tuple<int, int, double, double, int>> results;
    int iterations = 200;

    for (int n = minN; n <= maxN; ++n) {
        for (int k = minK; k <= std::min(n, maxK); ++k) {
            if (n == k) {
                continue; 
            }
            try {
                Matrix G = generateRandomMatrix(k, n);
                Matrix H = generateParityMatrix(G);
                
                // Run the encoding/decoding process for the specified number of iterations
                double totalEncodeTime = 0.0;
                double totalSyndromeTime = 0.0;
                int totalErrorsLeft = 0;

                for (int i = 0; i < iterations; ++i) {
                    auto startSyndrome = std::chrono::high_resolution_clock::now();
                    syndromesTable syndromes = generateSyndromes(n, k, H, false);
                    auto endSyndrome = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> syndromeDuration = endSyndrome - startSyndrome;
                    totalSyndromeTime += syndromeDuration.count();

                    Vec message = generateRandomVector(k);
                    Vec encodedMessage;
                    Vec receivedMessage;
                    Vec decodedMessage;

                    auto start = std::chrono::high_resolution_clock::now();
                    encodedMessage = encodeMessage(G, message, k, &bits);
                    receivedMessage = introduceErrors(encodedMessage, errorRate);
                    decodedMessage = decodeMessage(H, receivedMessage, syndromes, n, k);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> encodeDuration = end - start;
                    totalEncodeTime += encodeDuration.count();

                    int errorsLeft = getIntroducedErrorCount(message, decodedMessage);
                    totalErrorsLeft += errorsLeft;
                }

                double avgEncodeTime = totalEncodeTime / iterations;
                double avgSyndromeTime = totalSyndromeTime / iterations;
                int avgErrorsLeft = totalErrorsLeft / iterations;

                results.emplace_back(n, k, avgEncodeTime, avgSyndromeTime, avgErrorsLeft);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: " << e.what() << " for n=" << n << ", k=" << k << std::endl;
            }
        }
    }

    std::cout << "Benchmark Results:\n";
    std::cout << "n\tk\tEncode Time (s)\tSyndrome Time (s)\tErrors after decode\n";
    for (const auto& result : results) {
        int n, k, errorsFixed;
        double encodeTime, syndromeTime;
        std::tie(n, k, encodeTime, syndromeTime, errorsFixed) = result;
        std::cout << n << "\t" << k << "\t" << std::fixed << std::setprecision(7) << encodeTime << "\t" << syndromeTime << "\t" << errorsFixed << "\n";
    }
}