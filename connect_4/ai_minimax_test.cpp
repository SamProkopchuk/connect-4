#include "connect_4/ai_minimax.h"

#include <algorithm>

#include "connect_4/game.h"
#include "gtest/gtest.h"

namespace connect_4 {

namespace {

TEST(AIMinimaxTest, MinimaxTest) {
  constexpr int kDepth = 2;
  Game game;
  for (int i = 0; i < 3; ++i) {
    game.Play(1);
    if (i < 2) {
      game.Play(0);
    }
  }
  AIMinimax ai(game, kDepth);
  const int kMove = ai.GetMove();
  ASSERT_EQ(kMove, 1);
}

}  // namespace

}  // namespace connect_4

