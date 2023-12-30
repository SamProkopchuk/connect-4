#ifndef CONNECT4_UTIL_H_
#define CONNECT4_UTIL_H_

#include <cstdint>

namespace connect_4 {

constexpr int kNumRows = 6;
constexpr int kNumCols = 7;

// clang-format off
constexpr uint64_t kColMasks[kNumCols] = {0x3FULL,
                                          0x3FULL << kNumRows,
                                          0x3FULL << kNumRows * 2,
                                          0x3FULL << kNumRows * 3,
                                          0x3FULL << kNumRows * 4,
                                          0x3FULL << kNumRows * 5,
                                          0x3FULL << kNumRows * 6};

constexpr uint64_t kRowMasks[kNumRows] = {0x1041041041ULL,
                                          0x1041041041ULL << 1,
                                          0x1041041041ULL << 2,
                                          0x1041041041ULL << 3,
                                          0x1041041041ULL << 4,
                                          0x1041041041ULL << 5};
// clang-format on

// The board is represented by a uint64_t where the least significant kNumRows
// bits represent the 0th column, the next kNumRows bits represent the 1st
// column, and so on.
//
// For a given column, the least significant bit represents the first row, the
// second least significant bit represents the second row, and so on.
//
// For example: The uint64 represented by: 000...00111110 represents the board:
// 0000000
// 1000000
// 1000000
// 1000000
// 1000000
// 1000000
//
// Note a uint64_t contains 64 bits whereas a standard 6x7 connect-4 board
// contains 42 bits. Thus the first 22 bits are redundant and unused.

constexpr uint64_t kFullBoard = 0x3FFFFFFFFFFULL;

constexpr bool IsColFull(const uint64_t kBoard, const int kCol) {
  return (kBoard & kColMasks[kCol]) == kColMasks[kCol];
}

constexpr uint64_t GetMask(const int kRow, const int kCol) {
  return 1ULL << (kCol * kNumRows + kRow);
}

// Returns a mask of the piece position when dropped in the given column.
// Board contains both players' pieces.
uint64_t GetDropMask(uint64_t board, int col);

int GetRow(uint64_t normalized_piece_col);

bool IsWin(uint64_t pieces, int col);

}  // namespace connect_4

#endif  // CONNECT4_UTIL_H_
