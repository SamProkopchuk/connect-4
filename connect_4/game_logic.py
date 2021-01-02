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
            raise ValueError(f'Chip must be player 1 or 2\'s')
        if 0 not in self[:, column]:
            raise ValueError((f'Cannot place chip in column {column}: ' +
                              'it is already full'))
        row = np.argwhere(self[:, column] == 0).max()
        chip_idx = (row, column)
        self[chip_idx] = chip
        self._chip_idxs[chip].add(chip_idx)
        return chip_idx

    def isfull(board):
        return np.all(self)


class Player(metaclass=ABCMeta):
    def __init__(self, pnum):
        self._num = pnum
        self._board = None

    @property
    def num(self):
        return self._num
    
    @property
    def board(self):
        return self._board

    @board.setter
    def board(self, board: Board):
        assert isinstance(board, Board)
        self._board = board    

    @abstractmethod
    def move(self):
        '''
        Returns desired column to drop their chip.
        '''
        pass


class CLIPlayer(Player):
    def __init__(self, pnum):
        self._num = pnum

    def move(self):
        move = None
        while move is None:
            try:
                move = int(input('Where would you like to place the chip? (Column #): '))
                if (move not in range(self._board.shape[1]) or
                        0 not in self._board[:, move_]):
                    print('Move is invalid')
                    continue
            except ValueError:
                print('Please enter an integer')
        return move


class Game:
    def __init__(self, player_1: Player, player_2: Player):
        assert player_1.num == 1
        assert player_2.num == 2
        self.board = Board()
        player_1.board = self.board
        player_2.board = self.board
        self._pnum2player = {1: player_1, 2: player_2}
        self._winner = None

    def is_winning_move(self, move_idx) -> bool:
        if self._last_move == (None, None):
            return False
        lchip = self.board[self._last_move]
        chip_idxs = self.board.chip_idxs[lchip]
        lmrow, lmcol = self._last_move
        for dr, dc in product(range(2), range(2)):
            if dr == dc == 0:
                continue
            consec = 1
            for off in range(1, 4):
                if (lmrow + dr * off, lmcol + dc * off) not in chip_idxs:
                    break
                consec += 1
            for off in range(-1, -4, -1):
                if (lmrow + dr * off, lmcol + dc * off) not in chip_idxs:
                    break
                consec += 1
            if consec >= 4:
                return True
        return False

    def start(self, verbose=False):
        for pnum in cycle((1, 2)):
            col = self._pnum2player[pnum].move()
            last_move = self.board.place_chip(pnum, col)
            if verbose:
                print(self.board)
            if self.board.isfull():
                break
            if is_winning_move(last_move):
                self._winner = pnum

        if verbose:
            print('Tie' is self._winner is None else f'Player {self._winner} wins!')