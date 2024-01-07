#include "connect_4/ai_minimax.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <random>

#include "connect_4/util.h"

namespace connect_4 {

int AIMinimax::GetMove() {
  assert(kGame_.GetBoard() != kFullBoard);
  int best_col = -1;
  float alpha = std::numeric_limits<float>::lowest();
  float beta = std::numeric_limits<float>::max();
  Game game(kGame_);
  if (game.IsP1Turn()) {
    for (int col = 0; col < kNumCols; ++col) {
      if (IsColFull(kGame_.GetBoard(), col)) {
        continue;
      }
      if (game.Play(col) == GameResult::kP1Win) {
        return col;
      }
      const float kValue = Minimax(game, kDepth_ - 1, alpha, beta);
      game.Undo(col);
      if (kValue > alpha) {
        best_col = col;
        alpha = kValue;
      }
    }
  } else {
    for (int col = 0; col < kNumCols; ++col) {
      if (IsColFull(kGame_.GetBoard(), col)) {
        continue;
      }
      if (game.Play(col) == GameResult::kP2Win) {
        return col;
      }
      const float kValue = Minimax(game, kDepth_ - 1, alpha, beta);
      game.Undo(col);
      if (kValue < beta) {
        best_col = col;
        beta = kValue;
      }
    }
  }
  return best_col;
}

float AIMinimax::Minimax(Game& game, const int kDepth, float alpha,
                         float beta) {
  if (kDepth == 0 || game.GetBoard() == kFullBoard) {
    return 0;
  }
  if (game.IsP1Turn()) {
    // P1 is maximizing.
    float value = std::numeric_limits<float>::lowest();
    for (int col = 0; col < kNumCols; ++col) {
      if (IsColFull(game.GetBoard(), col)) {
        continue;
      }
      if (game.Play(col) == GameResult::kP1Win) {
        game.Undo(col);
        return 1.0 + 1e-4 * kDepth;
      }
      value = std::max(value, Minimax(game, kDepth - 1, alpha, beta));
      game.Undo(col);
      alpha = std::max(alpha, value);
      if (alpha >= beta) {
        break;
      }
    }
    return value;
  } else {
    float value = std::numeric_limits<int>::max();
    for (int col = 0; col < kNumCols; ++col) {
      if (IsColFull(game.GetBoard(), col)) {
        continue;
      }
      if (game.Play(col) == GameResult::kP2Win) {
        game.Undo(col);
        return -1.0 - 1e-4 * kDepth;
      }
      value = std::min(value, Minimax(game, kDepth - 1, alpha, beta));
      game.Undo(col);
      beta = std::min(beta, value);
      if (alpha >= beta) {
        break;
      }
    }
    return value;
  }
}

}  // namespace connect_4
