#include "../headers/benchmark.h"

void benchmark(int minN, int maxN, int minK, int maxK, double errorRate) {
    // Keeps track of added bits
    int bits = 0;
    // Results vector
    std::vector<std::tuple<int, int, double, double, double>> results;
    // Count of vectors that will be sent for each n x k combination
    int iterations = 200;

    for (int n = minN; n <= maxN; ++n) {
        for (int k = minK; k <= std::min(n, maxK); ++k) {
            std::cout << "Benchmarking for n=" << n << ", k=" << k << std::endl;

            try {
                // Create random generator and parity matrices
                Matrix G = generateRandomMatrix(k, n);
                Matrix H = generateParityMatrix(G);
                
                double totalEncodeTime = 0.0;
                double totalSyndromeTime = 0.0;
                int fixedVectors = 0;

                // Track how long it takes to generate syndromes
                auto startSyndrome = std::chrono::high_resolution_clock::now();
                syndromesTable syndromes = generateSyndromes(n, k, H, false);
                auto endSyndrome = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> syndromeDuration = endSyndrome - startSyndrome;
                totalSyndromeTime += syndromeDuration.count();

                // Run encoding / decoding
                for (int i = 0; i < iterations; ++i) {
                    std::cout << "Iteration " << i <<"/" << iterations << std::endl;

                    // Initialize vectors for encoding / decoding the message
                    Vec message = generateRandomVector(k);
                    Vec encodedMessage;
                    Vec receivedMessage;
                    Vec decodedMessage;

                    // Track how long encoding / sending / decoding process takes
                    auto start = std::chrono::high_resolution_clock::now();
                    encodedMessage = encodeMessage(G, message, k, &bits);
                    receivedMessage = introduceErrors(encodedMessage, errorRate);
                    decodedMessage = decodeMessage(H, receivedMessage, syndromes, n, k);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> encodeDuration = end - start;
                    totalEncodeTime += encodeDuration.count();

                    int errorsLeft = getIntroducedErrorCount(message, decodedMessage);
                    if (errorsLeft == 0) {
                        fixedVectors++;
                    }
                }

                double avgEncodeTime = totalEncodeTime / iterations;
                // double avgSyndromeTime = totalSyndromeTime / iterations;
                double fixedVectorPercentage = (static_cast<double>(fixedVectors) / iterations) * 100.0;

                results.emplace_back(n, k, avgEncodeTime, totalSyndromeTime, fixedVectorPercentage);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: " << e.what() << " for n=" << n << ", k=" << k << std::endl;
            }
        }
    }

    // Save results to a JSON file
    // Format file name based on benchmark
    int errorPercentage = static_cast<int>(errorRate * 100);
    std::ostringstream fileNameStream;
    fileNameStream << "benchmark_" << errorPercentage << ".json";
    std::string fileName = fileNameStream.str();

    // Save to file
    std::ofstream outFile(fileName);
    outFile << "{\n  \"results\": [\n";
    for (size_t i = 0; i < results.size(); ++i) {
        int n, k;
        double encodeTime, syndromeTime, fixedVectorPercentage;
        std::tie(n, k, encodeTime, syndromeTime, fixedVectorPercentage) = results[i];
        outFile << "    {\n";
        outFile << "      \"n\": " << n << ",\n";
        outFile << "      \"k\": " << k << ",\n";
        outFile << "      \"encode_time\": " << encodeTime << ",\n";
        outFile << "      \"syndrome_time\": " << syndromeTime << ",\n";
        outFile << "      \"fixed_vector_percentage\": " << fixedVectorPercentage << "\n";
        outFile << "    }";
        if (i < results.size() - 1) {
            outFile << ",";
        }
        outFile << "\n";
    }
    outFile << "  ]\n}";
    outFile.close();

    // Output to console
    std::cout << "Benchmark Results:\n";
    std::cout << "n\tk\tEncode Time (s)\tSyndrome Time (s)\tErrors after decode\n";
    for (const auto& result : results) {
        int n, k, errorsFixed;
        double encodeTime, syndromeTime;
        std::tie(n, k, encodeTime, syndromeTime, errorsFixed) = result;
        std::cout << n << "\t" << k << "\t" << std::fixed << std::setprecision(7) << encodeTime << "\t" << syndromeTime << "\t" << errorsFixed << "\n";
    }
}