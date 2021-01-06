from connect_4.game_logic import Game, CLIPlayer
from connect_4.ai import MiniMaxAI

def main():
    game = Game(MiniMaxAI, CLIPlayer)
    game.play(verbose=True)

if __name__ == '__main__':
    main()
