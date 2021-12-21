#include "func.h"
#include "display.h"

Game game_state;
player_stats stats{0, 0, 100};


int main() {

    int action{};
    coord_pair where{};
    int difficulty{};

    Matrix *Game = init_game(difficulty);
    
    unsigned int timer_start = time(NULL);

    while (game_state == BASIC || game_state == HP) {

        func_clear();
        display(Game);

        act(Game, action, where);

        switch (action) {

            case 1:
                dig(Game, where, false);
                break;

            case 2:
                flag_down(Game, where);
                break;

            case 3:
                flag_up(Game, where);
                break;
            
            case 4:
                stats.jokers++;
                dig(Game, where, true);
                break;

            case 5:
                matrix_destructor(Game);
                std::cout << "\nAu revoir !!!\n";
                return 0;

        }

        int required = Game->nb_row * Game->nb_column - difficulty;
        if (stats.discovered == required) {
            game_state = WON;
        }

        if (stats.player_health < 1) {
            game_state = LOST;
        }
    }

    unsigned int timer_end = time(NULL);
    int game_time = timer_end - timer_start;

    if (game_state == WON) {

        func_clear();
        display(Game);

        std::cout << "\n--- BRAVO ! VOUS AVEZ GAGNE !!! ---\n";
        std::cout << "Temps: " << game_time << "\n";
        std::cout << "Score: " << score(game_time) << "\n";

    } else if (game_state == LOST) {

        func_clear();
        display(Game);

        std::cout << "\n--- DOMMAGE ! VOUS AVEZ PERDU !!! ---\n";

    }

    matrix_destructor(Game);
    return 0; 
}
