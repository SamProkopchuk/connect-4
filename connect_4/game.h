#ifndef CONNECT_4_GAME_H_
#define CONNECT_4_GAME_H_

#include <cstdint>

namespace connect_4 {

enum class GameResult { kP1Win, kP2Win, kDraw, kInProgress };

class Game {
 public:
  Game() : p1_board_(0), p2_board_(0), p1_turn_(true) {}
  // Create the copy constructor:
  Game(const Game& kGame)
      : p1_board_(kGame.p1_board_),
        p2_board_(kGame.p2_board_),
        p1_turn_(kGame.p1_turn_) {}
  GameResult Play(int col);

  constexpr uint64_t GetBoard() const { return board_; }
  constexpr uint64_t GetP1Board() const { return p1_board_; }
  constexpr uint64_t GetP2Board() const { return p2_board_; }

 private:
  uint64_t p1_board_;
  uint64_t p2_board_;
  uint64_t board_;
  bool p1_turn_;
};

}  // namespace connect_4

#endif
