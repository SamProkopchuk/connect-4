import math
import numpy as np

from collections import deque
from game_logic import Board, Player
from typing import Tuple
from scipy.signal import convolve2d


class MiniMaxAI(Player):
    FILTERS = {
        2: [np.ones((1, 2)),
            np.ones((2, 1)),
            np.eye(2),
            np.eye(2)[::-1]],
        3: [np.ones((1, 3)),
            np.ones((3, 1)),
            np.eye(3),
            np.eye(3)[::-1]],
        4: [np.ones((1, 4)),
            np.ones((4, 1)),
            np.eye(4),
            np.eye(4)[::-1]]
    }

    CONSEC_WEIGHT = {
        2: 1,
        3: 1<<4,
        4: 1<<7
    }

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    @staticmethod
    def static_eval(board):
        '''Returns static evaluation of a connect 4 position'''
        res = 0
        p1view = (board == 1)
        p2view = (board == 2)
        for size in MiniMaxAI.FILTERS:
            for filter_ in MiniMaxAI.FILTERS[size]:
                p1score = (convolve2d(p1view, filter_, mode='valid') == size).sum()
                p2score = (convolve2d(p2view, filter_, mode='valid') == size).sum()
                res += (p1score - p2score) * MiniMaxAI.CONSEC_WEIGHT[size]
        return res

    @staticmethod
    def minimax(board, pnum, depth: int, maximize: bool):
        if depth <= 0:
            return MiniMaxAI.static_eval(board)

        weights = []
        for move in range(board.shape[1]):
            if board[:, move].all():
                continue
            row = np.argwhere(board[:, move] == 0).max()
            board[row, move] = pnum
            weights.append(MiniMaxAI.minimax(board, 1 + pnum % 2, depth-1, not maximize))
            board[row, move] = 0

        if not weights:
            # No moves means at given depth, the board became full.
            return 0
        elif maximize:
            return max(weights)
        else:
            return min(weights)

    def move(self):
        maximize = (self.num == 1)
        weight2move = {}
        board = self._board.copy()
        for move in range(board.shape[1]):
            if board[:, move].all():
                continue
            row = np.argwhere(board[:, move] == 0).max()
            board[row, move] = self.num
            weight2move[MiniMaxAI.minimax(board, 1 + self.num % 2, 3, maximize = not maximize)] = move
            board[row, move] = 0
        if maximize:
            move = weight2move[max(weight2move)]
        else:
            move = weight2move[min(weight2move)]
        return move
