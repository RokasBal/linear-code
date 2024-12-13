#ifndef DECODE_H
#define DECODE_H

#include "matrix.h"
#include "generateSyndromes.h"
std::vector<uint8_t> decodeMessage(const Matrix& H, const std::vector<uint8_t>& receivedMessage, syndromesTable syndromes, int n, int k);
std::vector<uint8_t> decodeBmpMessage(const Matrix& H, const std::vector<uint8_t>& receivedMessage, syndromesTable syndromes, int n, int k);

#endif // DECODE_H