#include "connect_4/game.h"

#include <cassert>

#include "connect_4/util.h"
#include "glog/logging.h"

namespace connect_4 {

std::ostream& operator<<(std::ostream& kStream, const Game& kGame) {
  std::string res;
  for (int row = 0; row < kNumRows; ++row) {
    for (int col = 0; col < kNumCols; ++col) {
      const uint64_t kMask = GetMask(row, col);
      if ((kGame.p1_board_ & kMask) != 0) {
        res += "X";
      } else if ((kGame.p2_board_ & kMask) != 0) {
        res += "O";
      } else {
        res += ".";
      }
    }
    if (row != kNumRows - 1) {
      res += "\n";
    }
  }
  return kStream << res;
}

GameResult Game::Play(int kCol) {
#ifndef NDEBUG
  const uint64_t kColMask = kColMasks[kCol];
  // Check there is at least one empty slot in the column.
  assert((board_ & kColMask) != kColMask);
#endif

  const uint64_t kDropMask = GetDropMask(board_, kCol);
  uint64_t& p_board = is_p1_turn_ ? p1_board_ : p2_board_;
  p_board |= kDropMask;
  board_ |= kDropMask;
  is_p1_turn_ = !is_p1_turn_;

  if (IsWin(p_board, kCol)) {
    return is_p1_turn_ ? GameResult::kP2Win : GameResult::kP1Win;
  } else if (board_ == kFullBoard) {
    return GameResult::kDraw;
  } else {
    return GameResult::kInProgress;
  }
}

}  // namespace connect_4
