#ifndef CONNECT_4_GAME_H_
#define CONNECT_4_GAME_H_

#include <cstdint>
#include <ostream>

#include "connect_4/util.h"

namespace connect_4 {

enum class GameResult { kP1Win, kP2Win, kDraw, kInProgress };

class Game {
 public:
  Game() : p1_board_(0), p2_board_(0), board_(0), is_p1_turn_(true) {}
  // Create the copy constructor:
  Game(const Game& kGame)
      : p1_board_(kGame.p1_board_),
        p2_board_(kGame.p2_board_),
        board_(kGame.board_),
        is_p1_turn_(kGame.is_p1_turn_) {}
  GameResult Play(const int kCol);

  [[nodiscard]] constexpr uint64_t GetP1Board() const { return p1_board_; }
  [[nodiscard]] constexpr uint64_t GetP2Board() const { return p2_board_; }
  [[nodiscard]] constexpr uint64_t GetBoard() const { return board_; }
  [[nodiscard]] constexpr bool IsP1Turn() const { return is_p1_turn_; }
  constexpr void Undo(const int kCol) {
    const uint64_t kMaskedCol = board_ & kColMasks[kCol];
    const uint64_t kLastPiece = kMaskedCol & -kMaskedCol;
    p1_board_ &= ~kLastPiece;
    p2_board_ &= ~kLastPiece;
    board_ &= ~kLastPiece;
    is_p1_turn_ = !is_p1_turn_;
  }

 private:
  friend std::ostream& operator<<(std::ostream& kStream, const Game& kGame);

  uint64_t p1_board_;
  uint64_t p2_board_;
  uint64_t board_;
  bool is_p1_turn_;
};

}  // namespace connect_4

#endif
