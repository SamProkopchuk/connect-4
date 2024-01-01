#include <iostream>

#include "connect_4/ai_minimax.h"
#include "connect_4/game.h"
#include "connect_4/util.h"

int main(int arc, char** argv) {
  constexpr int kDepth = 14;
  connect_4::Game game;
  connect_4::GameResult result = connect_4::GameResult::kInProgress;
  connect_4::AIMinimax ai(game, kDepth);
  std::cout << "Select player 1 (0) or player 2 (1):";
  int player;
  std::cin >> player;
  if (player == 1) {
    result = game.Play(ai.GetMove());
    std::cout << game << std::endl;
  }
  while (result == connect_4::GameResult::kInProgress) {
    if ((player == 0 && game.IsP1Turn()) || (player == 1 && !game.IsP1Turn())) {
      std::cout << "Choose a column (0-" << connect_4::kNumCols - 1 << "):";
      int col;
      std::cin >> col;
      result = game.Play(col);
    } else {
      result = game.Play(ai.GetMove());
    }
    std::cout << game << std::endl;
  }
  switch (result) {
    case connect_4::GameResult::kP1Win:
      std::cout << "P1 wins!" << std::endl;
      break;
    case connect_4::GameResult::kP2Win:
      std::cout << "P2 wins!" << std::endl;
      break;
    case connect_4::GameResult::kDraw:
      std::cout << "Draw!" << std::endl;
      break;
    default:
      std::cout << "Error!" << std::endl;
      break;
  }
}
