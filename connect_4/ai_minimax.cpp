#include "connect_4/ai_minimax.h"

#include <algorithm>
#include <cassert>
#include <limits>

#include "connect_4/util.h"

namespace connect_4 {

float AIMinimax::Minimax(const int kDepth, float alpha, float beta,
                         const bool kIsP1Turn) {
  if (kDepth == 0 || kGame_.GetBoard() == kFullBoard) {
    return 0;
  }
  if (kIsP1Turn) {
    // P1 is maximizing.
    float value = std::numeric_limits<float>::lowest();
    for (int col = 0; col < kNumCols; ++col) {
      if (IsColFull(kGame_.GetBoard(), col)) {
        continue;
      }
      const uint64_t kDropMask = GetDropMask(kGame_.GetBoard(), col);
      const uint64_t kNextBoard = kGame_.GetBoard() | kDropMask;
      value = std::max(value, Minimax(kDepth - 1, alpha, beta, false));
      alpha = std::max(alpha, value);
      if (alpha >= beta) {
        break;
      }
    }
    return value;
  } else {
    float value = std::numeric_limits<int>::max();
    for (int col = 0; col < kNumCols; ++col) {
      if (IsColFull(kGame_.GetBoard(), col)) {
        continue;
      }
      const uint64_t kDropMask = GetDropMask(kGame_.GetBoard(), col);
      const uint64_t kNextBoard = kGame_.GetBoard() | kDropMask;
      value = std::min(value, Minimax(kDepth - 1, alpha, beta, true));
      beta = std::min(beta, value);
      if (alpha >= beta) {
        break;
      }
    }
    return value;
  }
}

}  // namespace connect_4
