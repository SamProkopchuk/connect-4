from connect_4.game_logic import Game, CLIPlayer

def main():
    player_1 = CLIPlayer(1)
    player_2 = CLIPlayer(2)
    game = Game(player_1, player_2)
    game.play()

if __name__ == '__main__':
    main()
