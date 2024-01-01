#ifndef CONNECT4_AI_MINIMAX_H_
#define CONNECT4_AI_MINIMAX_H_

#include "connect_4/game.h"

namespace connect_4 {

class AIMinimax {
 public:
  AIMinimax(const Game& kGame, const int kDepth)
      : kGame_(kGame), kDepth_(kDepth) {}

  int GetMove();

 private:
  float Minimax(const Game& kGame, const int kDepth, float alpha, float beta);
  const Game& kGame_;
  const int kDepth_;
};

}  // namespace connect_4

#endif
