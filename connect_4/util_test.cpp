#include "util.h"

#include <algorithm>
#include <bitset>
#include <random>

#include "gtest/gtest.h"

namespace connect_4 {

namespace {

std::string BoardToString(const uint64_t kBoard) {
  std::string res;
  for (int row = 0; row < kNumRows; ++row) {
    for (int col = 0; col < kNumCols; ++col) {
      res += (kBoard & GetMask(row, col)) ? "X" : "O";
    }
    if (row != kNumRows - 1) {
      res += "\n";
    }
  }
  return res;
}

// A less efficient version of IsWin() for testing.
bool IsWinTest(const uint64_t kBoard, const int kRow, const int kCol) {
  if (!(kBoard & GetMask(kRow, kCol))) {
    return false;
  }
  // Check vertical.
  int num_below = 0;
  for (int row = kRow + 1; row < kNumRows; ++row) {
    const uint64_t kMask = GetMask(row, kCol);
    if ((kBoard & kMask) != kMask) {
      break;
    } else {
      ++num_below;
    }
  }
  if (num_below + 1 >= 4) {
    return true;
  }
  // Check horizontal.
  int num_right = 0;
  for (int col = kCol + 1; col < kNumCols; ++col) {
    const uint64_t kMask = GetMask(kRow, col);
    if ((kBoard & kMask) != kMask) {
      break;
    } else {
      ++num_right;
    }
  }
  int num_left = 0;
  for (int col = kCol - 1; col >= 0; --col) {
    const uint64_t kMask = GetMask(kRow, col);
    if ((kBoard & kMask) != kMask) {
      break;
    } else {
      ++num_left;
    }
  }
  if (num_left + num_right + 1 >= 4) {
    return true;
  }
  // Check diagonal. [\]
  int num_up_left = 0;
  for (int delta = 1; kRow - delta >= 0 && kCol - delta >= 0; ++delta) {
    const uint64_t kMask = GetMask(kRow - delta, kCol - delta);
    if ((kBoard & kMask) != kMask) {
      break;
    } else {
      ++num_up_left;
    }
  }
  int num_down_right = 0;
  for (int delta = 1; kRow + delta < kNumRows && kCol + delta < kNumCols;
       ++delta) {
    const uint64_t kMask = GetMask(kRow + delta, kCol + delta);
    if ((kBoard & kMask) != kMask) {
      break;
    } else {
      ++num_down_right;
    }
  }
  if (num_up_left + num_down_right + 1 >= 4) {
    return true;
  }
  // Check diagonal. [/]
  int num_up_right = 0;
  for (int delta = 1; kRow - delta >= 0 && kCol + delta < kNumCols; ++delta) {
    const uint64_t kMask = GetMask(kRow - delta, kCol + delta);
    if ((kBoard & kMask) != kMask) {
      break;
    } else {
      ++num_up_right;
    }
  }
  int num_down_left = 0;
  for (int delta = 1; kRow + delta < kNumRows && kCol - delta >= 0; ++delta) {
    const uint64_t kMask = GetMask(kRow + delta, kCol - delta);
    if ((kBoard & kMask) != kMask) {
      break;
    } else {
      ++num_down_left;
    }
  }
  if (num_up_right + num_down_left + 1 >= 4) {
    return true;
  }
  return false;
}

TEST(GameTest, kColMasksTest) {
  for (int col = 0; col < kNumCols; ++col) {
    const uint64_t kExpected = 0x3FULL << (col * kNumRows);
    ASSERT_EQ(kColMasks[col], kExpected);
  }
}

TEST(GameTest, kRowMasksTest) {
  const uint64_t kFirstRowMask = 0x1041041041ULL;
  for (int row = 0; row < kNumRows; ++row) {
    const uint64_t kExpected = kFirstRowMask << row;
    ASSERT_EQ(kRowMasks[row], kExpected);
  }
}

TEST(GameTest, GetMaskTest) {
  for (int row = 0; row < kNumRows; ++row) {
    for (int col = 0; col < kNumCols; ++col) {
      const uint64_t kMask = GetMask(row, col);
      const uint64_t kShifts = col * kNumRows + row;
      const uint64_t kExpected = 0x1ULL << kShifts;
      ASSERT_EQ(kMask, kExpected);
    }
  }
}

TEST(GameTest, GetRowTest) {
  for (int col = 0; col < kNumCols; ++col) {
    for (int row = 0; row < kNumRows; ++row) {
      const uint64_t kMask = GetMask(row, col);
      const int kRow = GetRow(kMask >> col * kNumRows);
      ASSERT_EQ(kRow, row);
    }
  }
}

TEST(GameTest, GetDropMaskTest) {
  std::array<int, 5> random_seeds = {42, 43, 44, 45, 46};
  for (int seed : random_seeds) {
    std::mt19937 gen(seed);
    int64_t board = 0x0ULL;
    std::vector<int> col_indices(kNumCols);
    std::iota(col_indices.begin(), col_indices.end(), 0);
    std::array<int, kNumCols> chips_in_col = {};
    int num_chips = 0;
    for (; col_indices.size() > 0; ++num_chips) {
      std::uniform_int_distribution<> dis(0, col_indices.size() - 1);
      const int kI = dis(gen);
      const int kDropColIdx = col_indices[kI];
      const uint64_t kColMask = kColMasks[kDropColIdx];
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

TEST(GameTest, IsWinTest) {
  const std::array<int, 5> kRandomSeeds = {42, 43, 44, 45, 46};
  const std::array<float, 7> kProbabilities = {0.1, 0.2, 0.4, 0.6,
                                               0.8, 0.9, 1.0};
  for (int seed : kRandomSeeds) {
    std::mt19937 gen(seed);
    for (float p : kProbabilities) {
      std::bernoulli_distribution dis(p);
      for (int min_row = kNumRows - 1; min_row >= 0; --min_row) {
        uint64_t board = 0x0ULL;
        for (int col = 0; col < kNumCols; ++col) {
          for (int row = min_row; row < kNumRows; ++row) {
            if (dis(gen)) {
              board |= GetMask(row, col);
            }
          }
        }
        for (int col = 0; col < kNumCols; ++col) {
          for (int row = 0; row < kNumRows; ++row) {
            if (board & GetMask(row, col)) {
              ASSERT_EQ(IsWin(board, col), IsWinTest(board, row, col));
              break;
            }
          }
        }
      }
    }
  }
}

}  // namespace

}  // namespace connect_4
