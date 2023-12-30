#ifndef CONNECT4_AI_MINIMAX_H_
#define CONNECT4_AI_MINIMAX_H_

#include "connect_4/game.h"

namespace connect_4 {

class AIMinimax {
 public:
  AIMinimax(const int kDepth, const Game& kGame)
      : kDepth_(kDepth), kGame_(kGame) {}

  int GetMove();

 private:
  float Minimax(const int kDepth, float alpha, float beta,
                const bool kIsP1Turn);
  const int kDepth_;
  const Game& kGame_;
};

}  // namespace connect_4

#endif
