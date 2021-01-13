from connect_4.game_logic import Game, CLIPlayer


def main():
    game = Game(CLIPlayer, CLIPlayer)
    game.play(verbose=True)


if __name__ == '__main__':
    main()
