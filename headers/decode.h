#ifndef DECODE_H
#define DECODE_H

// File used for decoding the message

#include "matrix.h"
#include "generateSyndromes.h"
#include <unordered_map>

// Decodes the message or it's part
std::vector<uint8_t> decodeMessage(const Matrix& H, const std::vector<uint8_t>& receivedMessage, syndromesTable syndromes, int n, int k);

#endif // DECODE_H