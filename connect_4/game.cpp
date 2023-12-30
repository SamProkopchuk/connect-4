#include "game.h"

#include <cassert>

#include "glog/logging.h"

namespace connect_4 {

GameResult Game::Play(int col) {
  const uint64_t kBoard = p1_board_ | p2_board_;
  const uint64_t kColMask = kColMasks[col];
  // Check there is at least one empty slot in the column.
  assert((kBoard & kColMask) != kColMask);

  const uint64_t kDropMask = GetDropMask(kBoard, col);
  p1_turn_ ? p1_board_ |= kDropMask : p2_board_ |= kDropMask;
  p1_turn_ = !p1_turn_;
  const uint64_t kNextBoard = p1_board_ | p2_board_;

  if (IsWin(kNextBoard, col)) {
    return p1_turn_ ? GameResult::kP2Win : GameResult::kP1Win;
  } else if (kNextBoard == kFullBoard) {
    return GameResult::kDraw;
  } else {
    return GameResult::kInProgress;
  }
}

}  // namespace connect_4

