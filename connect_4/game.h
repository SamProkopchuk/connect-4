#ifndef CONNECT_4_GAME_H_
#define CONNECT_4_GAME_H_

#include <cstdint>

namespace connect_4 {

enum class GameResult { kP1Win, kP2Win, kDraw, kInProgress };

class Game {
 public:
  Game(int player) : p1_board_(0), p2_board_(0), p1_turn_(true) {}
  GameResult Play(int col);

 private:
  uint64_t p1_board_;
  uint64_t p2_board_;
  bool p1_turn_;
};

}  // namespace connect_4

#endif
