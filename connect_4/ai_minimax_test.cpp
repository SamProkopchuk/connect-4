#include "connect_4/ai_minimax.h"

#include <algorithm>

#include "connect_4/game.h"
#include "gtest/gtest.h"

namespace connect_4 {

namespace {

TEST(AIMinimaxTest, MinimaxTest) {
  Game game;
  for (int i = 0; i < 3; ++i) {
    game.Play(1);
    if (i < 2) {
      game.Play(0);
    }
  }
  for (int depth = 2; depth < 10; ++depth) {
    ASSERT_EQ(AIMinimax(game, depth).GetMove(), 1);
  }
}

}  // namespace

}  // namespace connect_4

