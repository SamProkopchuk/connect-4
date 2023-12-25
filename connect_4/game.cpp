#include "game.h"

#include <cassert>

#include "glog/logging.h"

namespace connect_4 {

uint64_t GetColMask(int col) {
  // assert(col >= 0 && col < kNumCols);
  switch (col) {
    case 0:
      return kCol1Mask;
      break;
    case 1:
      return kCol2Mask;
      break;
    case 2:
      return kCol3Mask;
      break;
    case 3:
      return kCol4Mask;
      break;
    case 4:
      return kCol5Mask;
      break;
    case 5:
      return kCol6Mask;
      break;
    case 6:
      return kCol7Mask;
      break;
    default:
      return 0;
      break;
  }
}

uint64_t GetMask(int row, int col) { return 1ULL << (col * kNumRows + row); }

// Returns a mask of the piece position when dropped in the given column.
// Board contains both players' pieces.
uint64_t GetDropMask(uint64_t board, int col) {
  const uint64_t kColMask = GetColMask(col);
  const uint64_t kCol = board & kColMask;
  // Check there is at least one empty slot in the column.
  assert(kCol != kColMask);
  // Could probable use global constexprs for the 1 << (col * kNumRows) values.
  return kCol == 0 ? 1ULL << (col * kNumRows + 5) : (kCol & (-kCol)) >> 1;
}

void Game::Play(int col, double ms_to_think) {
  const uint64_t kPiecesMask = p1_board_ | p2_board_;
  // assert((kPiecesMask & GetMask(0, col)) == 0);
}

}  // namespace connect_4

