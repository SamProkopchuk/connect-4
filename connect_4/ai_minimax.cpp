#include "connect_4/ai_minimax.h"

#include <algorithm>
#include <cassert>
#include <limits>

#include "connect_4/util.h"

namespace connect_4 {

int AIMinimax::GetMove() {
  assert(kGame_.GetBoard() != kFullBoard);
  int best_col = -1;
  float alpha = std::numeric_limits<float>::lowest();
  float beta = std::numeric_limits<float>::max();
  if (kGame_.IsP1Turn()) {
    for (int col = 0; col < kNumCols; ++col) {
      if (IsColFull(kGame_.GetBoard(), col)) {
        continue;
      }
      Game game(kGame_);
      if (game.Play(col) == GameResult::kP1Win) {
        return col;
      }
      const float kValue = Minimax(game, kDepth_ - 1, alpha, beta);
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
      Game game(kGame_);
      if (game.Play(col) == GameResult::kP2Win) {
        return col;
      }
      const float kValue = Minimax(game, kDepth_ - 1, alpha, beta);
      if (kValue < beta) {
        best_col = col;
        beta = kValue;
      }
    }
  }
  return best_col;
}

float AIMinimax::Minimax(const Game& kGame, const int kDepth, float alpha,
                         float beta) {
  if (kDepth == 0 || kGame.GetBoard() == kFullBoard) {
    return 0;
  }
  if (kGame.IsP1Turn()) {
    // P1 is maximizing.
    float value = std::numeric_limits<float>::lowest();
    for (int col = 0; col < kNumCols; ++col) {
      if (IsColFull(kGame.GetBoard(), col)) {
        continue;
      }
      Game game(kGame);
      if (game.Play(col) == GameResult::kP1Win) {
        return 1.0;
      }
      value = std::max(value, Minimax(game, kDepth - 1, alpha, beta));
      alpha = std::max(alpha, value);
      if (alpha >= beta) {
        break;
      }
    }
    return value;
  } else {
    float value = std::numeric_limits<int>::max();
    for (int col = 0; col < kNumCols; ++col) {
      if (IsColFull(kGame.GetBoard(), col)) {
        continue;
      }
      Game game(kGame);
      if (game.Play(col) == GameResult::kP2Win) {
        return -1.0;
      }
      value = std::min(value, Minimax(game, kDepth - 1, alpha, beta));
      beta = std::min(beta, value);
      if (alpha >= beta) {
        break;
      }
    }
    return value;
  }
}

}  // namespace connect_4
