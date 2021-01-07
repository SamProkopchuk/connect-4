import math
import numpy as np

from collections import deque, defaultdict
from game_logic import Board, Player, Game, consec_direction
from typing import Tuple
from random import choice


class MiniMaxAI(Player):
    '''
    AI Player using the MiniMax Algorithm.
    Consecutive piece counts are used to evaluate a board position.
    '''
    CONSEC_WEIGHT = {
        1: 0,
        2: 1,
        3: 1 << 4,
        4: 1 << 7  # Not inf, to prefer future losses over an immediate loss.
    }

    def __init__(self, *args, maxdepth: int = 4, **kwargs):
        self.maxdepth = maxdepth
        super().__init__(*args, **kwargs)

    @staticmethod
    def static_eval(board: Board, whosmove: int) -> int:
        '''Returns static evaluation of a board given who's move it is'''
        res = 0

        for pnum in dict.fromkeys([whosmove, 1, 2]):
            coef = 1 if pnum == 1 else -1
            pchipidxs = board.chip_idxs[pnum]
            for r, c in pchipidxs:
                for dr, dc in ((0, 1), (1, 0), (1, 1), (-1, 1)):
                    consec = consec_direction(pchipidxs, r, c, dr, dc)
                    if consec >= 4:
                        return coef * MiniMaxAI.CONSEC_WEIGHT[4]
                    else:
                        res += coef * MiniMaxAI.CONSEC_WEIGHT[consec]
        return res

    def minimax(
            self,
            board: Board,
            pnum: int,
            depth: int,
            maximize: bool) -> int:
        if depth >= self.maxdepth:
            return MiniMaxAI.static_eval(board, pnum)
        elif Game.is_winner(1 + pnum % 2, board):
            # This is to ensure we don't calculate potential moves
            # for player x when player (1 + x % 2) has already won.
            return (1 if pnum == 2 else -1) * math.inf

        weights = []
        for move in range(board.shape[1]):
            if board[:, move].all():
                continue
            board.place_chip(pnum, move)
            weights.append(
                self.minimax(
                    board, 1 + pnum % 2,
                    depth + 1, not maximize))
            board.remove_chip(move)

        if maximize:
            return max(weights, default=0)
        else:
            return min(weights, default=0)

    def move(self):
        maximize = (self.num == 1)
        weight2move = defaultdict(list)
        board = self._board.copy()
        for move in range(board.shape[1]):
            if board[:, move].all():
                continue
            board.place_chip(self.num, move)
            weight2move[self.minimax(
                board, 1 + self.num % 2, 1, not maximize)].append(move)
            board.remove_chip(move)
        if maximize:
            move = choice(weight2move[max(weight2move)])
        else:
            move = choice(weight2move[min(weight2move)])
        return move
