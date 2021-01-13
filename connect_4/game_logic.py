import numpy as np

from abc import ABCMeta, abstractmethod
from copy import deepcopy
from typing import Tuple, DefaultDict, Dict, Optional, Type
from collections import defaultdict
from itertools import cycle


def consec_direction(pchipidxs: set, r: int, c: int, rd: int, cd: int) -> int:
    '''
    Returns the number of row column pairs
    with rd, cd (row delta, column delta) offsets from given r, c (row, column)
    '''
    consec = 1
    for coef in range(1, 4):
        if (r + coef * rd, c + coef * cd) not in pchipidxs:
            break
        consec += 1
    return consec


class Board(np.ndarray):
    '''Connect 4 board class'''
    def __new__(cls):
        return np.zeros((6, 7), dtype=np.uint8).view(cls)

    def __init__(self):
        self._chip_idxs: DefaultDict[int, set] = defaultdict(set)

    @property
    def chip_idxs(self) -> DefaultDict[int, set]:
        return self._chip_idxs

    def place_chip(self, chip: int, column: int) -> Tuple[int, int]:
        if chip not in (1, 2):
            raise ValueError(f'Chip must be player 1 or 2\'s')
        if self[:, column].all():
            raise ValueError((f'Cannot place chip in column {column}: ' +
                              'it is already full'))
        row = np.argwhere(self[:, column] == 0).max()
        chip_idx: Tuple[int, int] = (row, column)
        self[chip_idx] = chip
        self._chip_idxs[chip].add(chip_idx)
        return chip_idx

    def remove_chip(self, column: int) -> None:
        if not np.any(self[:, column]):
            raise ValueError('Cannot remove chip from empty column')
        row = np.argwhere(self[:, column]).min()
        chip = self[row, column]
        self[row, column] = 0
        self._chip_idxs[chip].remove((row, column))

    def isfull(self) -> bool:
        return self.all() # type: ignore

    def copy(self):
        res = np.copy(self).view(Board)
        res._chip_idxs = deepcopy(self._chip_idxs)
        return res


class Player(metaclass=ABCMeta):
    '''Parent class of any connect 4 player'''

    def __init__(self, pnum: int, board: Board, *args, **kwargs):
        self._num: int = pnum
        self._board: Board = board

    @property
    def num(self) -> int:
        return self._num

    @property
    def board(self) -> Board:
        return self._board

    @abstractmethod
    def move(self) -> int:
        '''
        Returns desired column to drop their chip.
        '''
        pass


class CLIPlayer(Player):
    '''Allows one to play a connect 4 game through the command line'''

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def move(self) -> int:
        move = None
        while move is None:
            try:
                move = int(input('Enter a column number to place chip: '))
                if (not 0 <= move < self._board.shape[1] or
                        np.all(self._board[:, move])):
                    print('Move is invalid')
                    move = None
            except ValueError:
                print('Please enter an integer')
        return move


class Game:
    '''Connect 4 game class. Creates and plays connect 4 game instances'''

    def __init__(
            self,
            p1_cls: Type[Player],
            p2_cls: Type[Player],
            p1_kwargs: Dict[str, object]={},
            p2_kwargs: Dict[str, object]={}):
        self._board: Board = Board()
        self._pnum2player: Dict[int, Player] = {
            1: p1_cls(1, self._board, **p1_kwargs),
            2: p2_cls(2, self._board, **p2_kwargs)}
        self._winner: Optional[int] = None

    @property
    def board(self):
        return self._board

    @property
    def winner(self) -> Optional[int]:
        return self._winner

    @staticmethod
    def is_winner(pnum: int, board: Board) -> bool:
        '''Returns static evaluation of a board given who's move it is'''
        pchipidxs = board.chip_idxs[pnum]
        for r, c in pchipidxs:
            for dr, dc in ((0, 1), (1, 0), (1, 1), (-1, 1)):
                consec = consec_direction(pchipidxs, r, c, dr, dc)
                if consec >= 4:
                    return True
        return False

    def play(self, verbose=False) -> None:
        for pnum in cycle((1, 2)):
            column = self._pnum2player[pnum].move()
            last_move = self._board.place_chip(pnum, column)
            if verbose:
                print((f'Player {pnum} placed a chip in column {last_move[1]}' +
                       f' -> row {last_move[0]}'))
                print(self._board)
            if Game.is_winner(pnum, self._board):
                self._winner = pnum
                break
            elif self._board.isfull():
                break
        if verbose:
            print('Tie' if self._winner is None else f'Player {self._winner} wins!')
