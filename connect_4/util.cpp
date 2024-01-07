#include "connect_4/util.h"

#include <algorithm>
#include <cassert>

namespace connect_4 {

namespace {

bool IsVerticalWin(const uint64_t kPieces, const int kRow, const int kCol) {
  if (kRow <= 2) {
    // Check vertical.
    const uint64_t kVerticalMask = 0xFULL << (kRow + kCol * kNumRows);
    if ((kPieces & kVerticalMask) == kVerticalMask) {
      return true;
    }
  }
  return false;
}

bool IsHorizontalWin(const uint64_t kPieces, const int kRow, const int kCol) {
  // Check horizontal.
  const uint64_t kMaskedRow = kPieces & kRowMasks[kRow];
  const int kMinCol = std::max(0, kCol - 3);
  const int kMaxCol = std::min(kCol, kNumCols - 3);
  for (int kCol = kMinCol; kCol <= kMaxCol; ++kCol) {
    const uint64_t kHorizontalMask = 0x41041ULL << (kRow + kCol * kNumRows);
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
bool IsTLBRDiagonalWin(const uint64_t kPieces, const int kRow,
                                 const int kCol) {
  // Check diagonal. [\]
  const int kMinCol = std::max(0, kCol - 3);
  const int kMinRow = std::max(0, kRow - 3);
  const int kMaxCol = std::min(kCol, kNumCols - 4);
  const int kMaxRow = std::min(kRow, kNumRows - 4);
  const int kDelta0 = std::max(kMinCol - kCol, kMinRow - kRow);
  const int kDeltaF = std::min(kMaxCol - kCol, kMaxRow - kRow);
  for (int delta = kDelta0; delta <= kDeltaF; ++delta) {
    const int kCurrRow = kRow + delta;
    const int kCurrCol = kCol + delta;
    const uint64_t kMask = 0x204081ULL << (kCurrCol * kNumRows + kCurrRow);
    if ((kPieces & kMask) == kMask) {
      return true;
    }
  }
  return false;
}

// The mask "0x4210800000ULL" represents:
// 0000000
// 0000000
// 0000001
// 0000010
// 0000100
// 0001000
bool IsBLTRDiagonalWin(const uint64_t kPieces, const int kRow, const int kCol) {
  // Check diagonal. [/]
  const int kMinCol = std::max(kCol, 3);
  const int kMinRow = std::max(kRow - 3, 0);
  const int kMaxCol = std::min(kCol + 3, kNumCols - 1);
  const int kMaxRow = std::min(kRow, kNumRows - 4);
  const int kDelta0 = std::max(kMinCol - kCol, kRow - kMaxRow);
  const int kDeltaF = std::min(kMaxCol - kCol, kRow - kMinRow);
  for (int delta = kDelta0; delta <= kDeltaF; ++delta) {
    const int kCurrRow = kRow - delta;
    const int kCurrCol = kCol + delta;
    const uint64_t kMask =
        0x4210800000ULL >> (38 - (kCurrCol * kNumRows + kCurrRow));
    if ((kPieces & kMask) == kMask) {
      return true;
    }
  }
  return false;
}

}  // namespace

std::string BoardToString(const uint64_t kBoard) {
  std::string res;
  for (int row = 0; row < kNumRows; ++row) {
    for (int col = 0; col < kNumCols; ++col) {
      res += (kBoard & GetMask(row, col)) ? "X" : "O";
    }
    if (row != kNumRows - 1) {
      res += "\n";
    }
  }
  return res;
}

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
int GetRow(const uint64_t kMaskedCol) {
  assert(kMaskedCol != 0);
  return std::countr_zero(kMaskedCol) % kNumRows;
}

bool IsWin(const uint64_t kPieces, const int kCol) {
  const uint64_t kColMask = kColMasks[kCol];
  const uint64_t kMaskedCol = kPieces & kColMask;
  const int kRow = GetRow(kMaskedCol);
  return IsVerticalWin(kPieces, kRow, kCol) ||
         IsHorizontalWin(kPieces, kRow, kCol) ||
         IsTLBRDiagonalWin(kPieces, kRow, kCol) ||
         IsBLTRDiagonalWin(kPieces, kRow, kCol);
}

}  // namespace connect_4
