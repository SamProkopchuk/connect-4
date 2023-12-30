#include "connect_4/util.h"

#include <algorithm>
#include <cassert>

namespace connect_4 {

namespace {

bool IsVerticalWin(uint64_t pieces, int row, int col) {
  if (row <= 2) {
    // Check vertical.
    const uint64_t kVerticalMask = 0xFULL << (row + col * kNumRows);
    if ((pieces & kVerticalMask) == kVerticalMask) {
      return true;
    }
  }
  return false;
}

bool IsHorizontalWin(uint64_t pieces, int row, int col) {
  // Check horizontal.
  const uint64_t kMaskedRow = pieces & kRowMasks[row];
  const int kMinCol = std::max(0, col - 3);
  const int kMaxCol = std::min(col, kNumCols - 3);
  for (int col = kMinCol; col <= kMaxCol; ++col) {
    const uint64_t kHorizontalMask = 0x41041ULL << (row + col * kNumRows);
    if ((kMaskedRow & kHorizontalMask) == kHorizontalMask) {
      return true;
    }
  }
  return false;
}

// The mask "0x204081ULL" represents:
// 1000000
// 0100000
// 0010000
// 0001000
// 0000000
// 0000000
bool IsTLBRDiagonalWin(uint64_t pieces, int row, int col) {
  // Check diagonal. [\]
  const int kMinCol = std::max(0, col - 3);
  const int kMinRow = std::max(0, row - 3);
  const int kMaxCol = std::min(col - 3, kNumCols - 6) + 3;
  const int kMaxRow = std::min(row - 3, kNumRows - 6) + 3;
  const int kDelta0 = std::max(kMinCol - col, kMinRow - row);
  const int kDeltaF = std::min(kMaxCol - col, kMaxRow - row);
  for (int delta = kDelta0; delta <= kDeltaF; ++delta) {
    const int kRow = row + delta;
    const int kCol = col + delta;
    const uint64_t kMask = 0x204081ULL << (kCol * kNumRows + kRow);
    if ((pieces & kMask) == kMask) {
      return true;
    }
  }
  return false;
}

// The mask "0x1084200000ULL" represents:
// 0000001
// 0000010
// 0000100
// 0001000
// 0000000
// 0000000
bool IsBLTRDiagonalWin(uint64_t pieces, int row, int col) {
  // Check diagonal. [/]

  const int kMinCol = std::max(col - 3, 0) + 3;
  const int kMinRow = std::max(row - 3, 0);
  const int kMaxCol = std::min(col + 3, kNumCols - 1);
  const int kMaxRow = std::min(row + 3, kNumRows - 1) - 3;
  const int kDelta0 = std::max(kMinCol - col, row - kMaxRow);
  const int kDeltaF = std::min(kMaxCol - col, row - kMinRow);
  for (int delta = kDelta0; delta <= kDeltaF; ++delta) {
    const int kRow = row - delta;
    const int kCol = col + delta;
    const uint64_t kMask = 0x4210800000ULL >> (38 - (kCol * kNumRows + kRow));
    if ((pieces & kMask) == kMask) {
      return true;
    }
  }
  return false;
}

}  // namespace

// Returns a mask of the piece position when dropped in the given column.
// Board contains both players' pieces.
uint64_t GetDropMask(uint64_t board, int col) {
  const uint64_t kColMask = kColMasks[col];
  const uint64_t kMaskedCol = board & kColMask;
  // Check there is at least one empty slot in the column.
  assert(kMaskedCol != kColMask);
  // Could probable use global constexprs for the 1 << (col * kNumRows) values.
  return kMaskedCol == 0 ? kColMask & kRowMasks[kNumRows - 1]
                         : (kMaskedCol & (-kMaskedCol)) >> 1;
}

// Returns the row index of the last piece dropped in the given normalized col.
// Normalized means it's at col 0.
int GetRow(uint64_t normalized_piece_col) {
  assert(normalized_piece_col != 0);
  // Count leading zeros of the normalized column.
  int leading_zeros = 0;
  while ((normalized_piece_col & 0x1ULL) != 0x1ULL) {
    normalized_piece_col >>= 1;
    ++leading_zeros;
  }
  return leading_zeros;
}

bool IsWin(uint64_t pieces, const int kCol) {
  const uint64_t kColMask = kColMasks[kCol];
  const uint64_t kMaskedCol = pieces & kColMask;
  const int kRow = GetRow(kMaskedCol >> (kCol * kNumRows));
  return IsVerticalWin(pieces, kRow, kCol) ||
         IsHorizontalWin(pieces, kRow, kCol) ||
         IsTLBRDiagonalWin(pieces, kRow, kCol) ||
         IsBLTRDiagonalWin(pieces, kRow, kCol);
}

}  // namespace connect_4
