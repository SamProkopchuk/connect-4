import math
import numpy as np

from collections import deque, defaultdict
from .game_logic import Board, Player, Game, consec_direction
from typing import Tuple, Optional
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
        4: 1 << 8
    }

    def __init__(self, *args, maxdepth: int=5, **kwargs):
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
                    res += coef * MiniMaxAI.CONSEC_WEIGHT[consec]
        return res

    def minimax(
            self,
            board: Board,
            pnum: int,
            depth: int,
            maximize: bool,
            alpha: int,
            beta: int) -> Tuple[int, Optional[int]]:
        '''
        alpha -> best maximizer value.
        beta  -> best minimizer value.
        '''
        # Player number of the other player (1 <-> 2):
        onum = 1 + pnum % 2
        if Game.is_winner(onum, board):
            # This is to ensure we don't calculate potential moves
            # for player pnum when previous player already won.
            # AKA: Is a terminal node
            return (1 if onum == 1 else -1) * MiniMaxAI.CONSEC_WEIGHT[4], None
        if depth >= self.maxdepth:
            return MiniMaxAI.static_eval(board, pnum), None

        bestweight = -math.inf if maximize else math.inf
        bestmove = None
        for move in range(board.shape[1]):
            if board[0, move] == 0:
                # ^If this column on the board isn't full
                board.place_chip(pnum, move)
                weight, _ = self.minimax(
                    board, onum,
                    depth + 1, not maximize,
                    alpha, beta)
                board.remove_chip(move)
                if maximize:
                    if weight > bestweight:
                        bestweight, bestmove = weight, move
                    alpha = max(bestweight, alpha) # type: ignore
                else:
                    if weight < bestweight:
                        bestweight, bestmove = weight, move
                    beta = min(bestweight, beta) # type: ignore
                if beta <= alpha:
                    break
        if bestmove is None:
            # This means no moves are valid aka the board is full.
            return 0, None
        else:
            return bestweight, bestmove # type: ignore

    def move(self):
        board = self._board.copy()
        eval_, move = self.minimax(
            board, self.num, depth=0, maximize=(self.num == 1),
            alpha=-math.inf, beta=math.inf)
        return move
