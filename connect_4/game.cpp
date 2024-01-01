#include "connect_4/game.h"

#include <cassert>

#include "connect_4/util.h"
#include "glog/logging.h"

namespace connect_4 {

GameResult Game::Play(int kCol) {
  const uint64_t kColMask = kColMasks[kCol];
  // Check there is at least one empty slot in the column.
  assert((board_ & kColMask) != kColMask);

  const uint64_t kDropMask = GetDropMask(board_, kCol);
  uint64_t &p_board = is_p1_turn_ ? p1_board_ : p2_board_;
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

