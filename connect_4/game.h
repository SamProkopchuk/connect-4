#ifndef CONNECT_4_GAME_H_
#define CONNECT_4_GAME_H_

#include <cstdint>

namespace connect_4 {

constexpr int kNumRows = 6;
constexpr int kNumCols = 7;

constexpr uint64_t kCol1Mask = 63ULL;
constexpr uint64_t kCol2Mask = kCol1Mask << kNumRows;
constexpr uint64_t kCol3Mask = kCol2Mask << kNumRows;
constexpr uint64_t kCol4Mask = kCol3Mask << kNumRows;
constexpr uint64_t kCol5Mask = kCol4Mask << kNumRows;
constexpr uint64_t kCol6Mask = kCol5Mask << kNumRows;
constexpr uint64_t kCol7Mask = kCol6Mask << kNumRows;

uint64_t GetColMask(int col);

uint64_t GetMask(int row, int col);

// Returns a mask of the piece position when dropped in the given column.
// Board contains both players' pieces.
uint64_t GetDropMask(uint64_t board, int col);

class Game {
 public:
  Game(int player) : p1_board_(0), p2_board_(0) {}
  void Play(int col, double ms_to_think);

 private:
  uint64_t p1_board_;
  uint64_t p2_board_;
};

}  // namespace connect_4

#endif
