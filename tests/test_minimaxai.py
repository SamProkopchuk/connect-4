from connect_4 import Game, MiniMaxAI


def main():
    game = Game(MiniMaxAI, MiniMaxAI)
    game.play(verbose=True)


if __name__ == '__main__':
    main()
