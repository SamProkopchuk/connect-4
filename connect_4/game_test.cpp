#include "connect_4/game.h"

#include <bitset>
#include <random>
#include <string>

#include "connect_4/util.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace connect_4 {

namespace {

TEST(GameTest, PlayTest) {
  Game game;
  bool is_p1_turn = true;
  for (int i = 0; i < 3; ++i) {
    ASSERT_EQ(game.IsP1Turn(), is_p1_turn);
    ASSERT_EQ(game.Play(0), GameResult::kInProgress);
    is_p1_turn = !is_p1_turn;
    ASSERT_EQ(game.IsP1Turn(), is_p1_turn);
    ASSERT_EQ(game.Play(1), GameResult::kInProgress);
    is_p1_turn = !is_p1_turn;
  }
  ASSERT_EQ(game.IsP1Turn(), is_p1_turn);
  ASSERT_EQ(game.Play(0), GameResult::kP1Win);
  is_p1_turn = !is_p1_turn;
}

TEST(GameTest, PlayTest2) {
  Game game;
  bool is_p1_turn = true;
  for (int row = kNumRows - 1; row >= kNumRows - 3; --row) {
    for (int col = 0; col < kNumCols; ++col) {
      ASSERT_EQ(game.IsP1Turn(), is_p1_turn);
      ASSERT_EQ(game.Play(col), GameResult::kInProgress);
      is_p1_turn = !is_p1_turn;
    }
  }
  ASSERT_EQ(game.IsP1Turn(), is_p1_turn);
  ASSERT_EQ(game.Play(0), GameResult::kP2Win);
  is_p1_turn = !is_p1_turn;
}

}  // namespace
}  // namespace connect_4
