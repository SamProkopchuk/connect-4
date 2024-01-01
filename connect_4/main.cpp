#include <iostream>

#include "connect_4/ai_minimax.h"
#include "connect_4/game.h"
#include "connect_4/util.h"
#include "glog/logging.h"
#include "glog/stl_logging.h"

namespace {

using GameState = std::pair<uint64_t, uint64_t>;
using CumulativeResult = std::array<int, 3>;

using CumulativeResults = std::map<GameState, CumulativeResult>;

}  // namespace

int main(int arc, char** argv) {
  constexpr int kDepth = 10;
  constexpr int kNumTrials = 1;
  CumulativeResults cumulative_results;
  for (int i = 0; i < kNumTrials; ++i) {
    connect_4::Game game;
    connect_4::GameResult result = connect_4::GameResult::kInProgress;
    connect_4::AIMinimax ai(game, kDepth);
    std::vector<GameState> game_states;
    game_states.push_back({game.GetP1Board(), game.GetP2Board()});
    while (result == connect_4::GameResult::kInProgress) {
      result = game.Play(ai.GetMove());
      game_states.push_back({game.GetP1Board(), game.GetP2Board()});
    }
    std::cout << game << std::endl;
    std::cout << ((result == connect_4::GameResult::kP1Win)
                      ? "Player 1 wins!"
                      : ((result == connect_4::GameResult::kP2Win)
                             ? "Player 2 wins!"
                             : "Draw!"))
              << "\n";
    while (game_states.size() > 0) {
      GameState game_state = game_states.back();
      game_states.pop_back();
      switch (result) {
        case connect_4::GameResult::kP1Win:
          ++cumulative_results[game_state][2];
          break;
        case connect_4::GameResult::kP2Win:
          ++cumulative_results[game_state][0];
          break;
        case connect_4::GameResult::kDraw:
          ++cumulative_results[game_state][1];
          break;
        default:
          std::cout << "Error!" << std::endl;
          break;
      }
    }
  }
}
