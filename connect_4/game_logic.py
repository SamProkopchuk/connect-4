import numpy as np

from abc import ABCMeta, abstractmethod
from typing import Tuple, DefaultDict, Dict, Optional
from collections import defaultdict
from itertools import cycle


class Board(np.ndarray):
    def __new__(cls):
        return np.zeros((6, 7)).view(cls)

    def __init__(self):
        self._chip_idxs: DefaultDict[int, set] = defaultdict(set)

    @property
    def chip_idxs(self) -> DefaultDict[int, set]:
        return self._chip_idxs

    def place_chip(self, chip: int, column: int) -> Tuple[int, int]:
        if chip not in (1, 2):
            raise ValueError(f'Chip must be player 1 or 2\'s')
        if 0 not in self[:, column]:
            raise ValueError((f'Cannot place chip in column {column}: ' +
                              'it is already full'))
        row = np.argwhere(self[:, column] == 0).max()
        chip_idx: Tuple[int, int] = (row, column)
        self[chip_idx] = chip
        self._chip_idxs[chip].add(chip_idx)
        return chip_idx

    def isfull(self) -> bool:
        return self.all() # type: ignore


class Player(metaclass=ABCMeta):
    def __init__(self, pnum: int):
        self._num: int = pnum
        self._board: Optional[Board] = None

    @property
    def num(self) -> int:
        return self._num

    @property
    def board(self) -> Optional[Board]:
        return self._board

    @board.setter
    def board(self, board: Board) -> None:
        assert isinstance(board, Board)
        self._board = board

    @abstractmethod
    def move(self) -> int:
        '''
        Returns desired column to drop their chip.
        '''
        pass


class CLIPlayer(Player):
    def __init__(self, pnum):
        super().__init__(pnum)

    def move(self) -> int:
        if self._board is None:
            raise ValueError('Player must be given a board to play moves')
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
    def __init__(self, player_1: Player, player_2: Player):
        assert player_1.num == 1
        assert player_2.num == 2
        self.board: Board = Board()
        player_1.board = self.board
        player_2.board = self.board
        self._pnum2player: Dict[int, Player] = {1: player_1, 2: player_2}
        self._winner: Optional[int] = None

    @property
    def winner(self) -> Optional[int]:
        return self._winner

    def is_winning_move(self, move_idx: Tuple[int, int]) -> bool:
        lchip = self.board[move_idx]
        chip_idxs = self.board.chip_idxs[lchip]
        lmrow, lmcol = move_idx
        for dr, dc in ((1, 0), (1, 1), (0, 1), (1, -1)):
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

    def play(self, verbose=False) -> None:
        for pnum in cycle((1, 2)):
            column = self._pnum2player[pnum].move()
            last_move = self.board.place_chip(pnum, column)
            if verbose:
                print(self.board)
            if self.is_winning_move(last_move):
                self._winner = pnum
                break
            if self.board.isfull():
                break
        if verbose:
            print('Tie' if self._winner is None else f'Player {self._winner} wins!')
