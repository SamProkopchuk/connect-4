import numpy as np

from abc import ABCMeta, abstractmethod
from typing import Tuple
from collections import defaultdict
from itertools import product, cycle


class Board(np.ndarray):
    def __new__(cls):
        return np.zeros((6, 7)).view(cls)

    def __init__(self):
        self._chip_idxs = defaultdict(set)

    @property
    def chip_idxs(self):
        return self._chip_idxs

    def place_chip(self, chip: int, column: int) -> Tuple[int]:
        if chip not in (1, 2):
            raise ValueError(f'Chip must be player {1} or {2}\'s')
        if 0 not in self[:, column]:
            raise ValueError((f'Cannot place chip in column {column}: ' +
                              'it is already full'))
        row = np.argwhere(self[:, column] == 0).max()
        chip_idx = (row, column)
        self[chip_idx] = chip
        self._chip_idxs[chip].add(chip_idx)
        return chip_idx

    def isfull(board):
        return np.any(self == 0)


class Player(metaclass=ABCMeta):
    def __init__(self, pnum, board):
        self._num = pnum
        self._board = board

    @abstractmethod
    def move(self):
        '''
        Returns desired column to drop their chip.
        '''
        pass


class Game:
    def __init__(self, player_1: Player, player_2: Player):
        assert isinstance(player_1, Player)
        assert isinstance(player_2, Player)
        self.board = Board()

        player_1.setboard(self.board)
        player_2.setboard(self.board)
        self._pnum2player = {1: player_1, 2: player_2}
        self._last_move = (None, None)
        self._winner = None

    def is_last_move_winning(self) -> bool:
        if self._last_move == (None, None):
            return False
        lmrow, lmcol = self.board[self._last_move]
        chip_idxs = self.board.chip_idxs[chip]
        for dr, dc in product(range(2), range(2)):
            if dr == dc == 0:
                continue
            consec = 1
            for off in range(1, 4):
                if (lmrow + dr * off, lmrow + dc * off) not in chip_idxs:
                    break
                consec += 1
            for off in range(-1, -4, -1):
                if (lmrow + dr * off, lmrow + dc * off) not in chip_idxs:
                    break
                consec += 1
            if consec >= 4:
                return True
        return False

    def start(self, verbose=False):
        for pnum in cycle((1, 2)):
            col = self._pnum2player[pnum].move()
            self._last_move = self.board.place_chip(pnum, col)
            if verbose:
                print(self.board)
            if isboardfull():
                break
            if is_last_move_winning():
                self._winner = pnum
