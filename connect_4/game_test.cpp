#include "game.h"

#include <bitset>
#include <random>

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace connect_4 {

namespace {

TEST(GameTest, GetColMaskTest) {
  for (int col = 0; col < kNumCols; ++col) {
    const uint64_t kMask = GetColMask(col);
    const uint64_t kShifts = col * kNumRows;
    const uint64_t kExpected = 63ULL << kShifts;
    ASSERT_EQ(kMask, kExpected);
  }
}

TEST(GameTest, GetMaskTest) {
  for (int row = 0; row < kNumRows; ++row) {
    for (int col = 0; col < kNumCols; ++col) {
      const uint64_t kMask = GetMask(row, col);
      const uint64_t kShifts = col * kNumRows + row;
      const uint64_t kExpected = 1ULL << kShifts;
      ASSERT_EQ(kMask, kExpected);
    }
  }
}

TEST(GameTest, GetDropMaskTest) {
  std::vector<int> random_seeds = {42, 43, 44, 45, 46};
  for (int seed : random_seeds) {
    std::mt19937 gen(seed);
    int64_t board = 0ULL;
    std::vector<int> col_indices(kNumCols);
    std::iota(col_indices.begin(), col_indices.end(), 0);
    std::array<int, kNumCols> chips_in_col = {};
    int num_chips = 0;
    for (; col_indices.size() > 0; ++num_chips) {
      std::uniform_int_distribution<> dis(0, col_indices.size() - 1);
      const int kI = dis(gen);
      const int kDropColIdx = col_indices[kI];
      const uint64_t kColMask = GetColMask(kDropColIdx);
      const uint64_t kCol = board & kColMask;
      ASSERT_TRUE(kCol != kColMask);
      const int kDropRowIdx = kNumRows - 1 - chips_in_col[kDropColIdx];
      ++chips_in_col[kDropColIdx];
      if (chips_in_col[kDropColIdx] == kNumRows) {
        col_indices.erase(col_indices.begin() + kI);
      }
      const uint64_t kDropMask = GetDropMask(board, kDropColIdx);
      ASSERT_EQ(kDropMask, GetMask(kDropRowIdx, kDropColIdx));
      board |= kDropMask;
    }
    ASSERT_EQ(num_chips, kNumRows * kNumCols);
  }
}

}  // namespace

}  // namespace connect_4
