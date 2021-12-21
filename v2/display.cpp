#include "display.h"

using std::cout;
using std::cin;

extern Game game_state;
extern player_stats stats;

void rules();


static bool menu_condition(int size, int d) {
    bool condition = (
        d < 1 || d > 8 || 
        (size == 1 && d > 2) ||
        (size == 2 && d > 5) ||
        (size == 3 && d > 4) ||
        (size == 4 && d > 6)
    );
    return condition;
}

void menu(int &row, int &col, int &d) {
    int index_size{}, index_d{}, index_game{};
    bool rules_read = false;

    func_clear();
    while (rules_read == false) {

        func_clear();
        cout << "\n\n\n\n\nQuel version du jeu voulez-vous jouer: \n";
        cout << "1.           Basique\n";
        cout << "2.           Avec de la vie\n\n";
        cout << "3.           Regles du jeu...\n\n";
        cout << " -> ";
        cin >> index_game;
        while(index_game < 1 || index_game > 3 || !cin) {
            cin.clear();
            cin.ignore();
            cout << "-> ";
            cin >> index_game;
        }

        if (index_game == 3) {
            rules();
        } else {
            rules_read = true;
        }
    }

    func_clear();
    cout << "\n\n\n\n\nChoisissez une taille de jeu: \n";
    cout << "1.           10 x 10\n";
    cout << "2.           15 x 15\n";
    cout << "3.           10 x 20\n";
    cout << "4.           20 x 20\n";
    cout << "5.           20 x 30\n\n";
    cout << "Taille -> ";
    cin >> index_size;
    while(index_size < 1 || index_size > 5 || !cin) {
        cin.clear();
        cin.ignore();
        cout << "-> ";
        cin >> index_size;
    }

    func_clear();
    cout << "\n\n\n\n\nChoisissez la difficulte du jeu: \n";
    cout << "1.           10 mines\n";
    cout << "2.           50 mines\n";
    cout << "3.           100 mines\n";
    cout << "4.           150 mines\n";
    cout << "5.           200 mines\n";
    cout << "6.           300 mines\n";
    cout << "7.           400 mines\n";
    cout << "8.           500 mines\n\n";
    cout << "Difficulte -> ";
    cin >> index_d;
    while(menu_condition(index_size, index_d) || !cin) {
        cin.clear();
        cin.ignore();
        cout << "-> ";
        cin >> index_d;
    }

    switch (index_game) {
        case 1:
            game_state = BASIC;
            break;
        case 2:
            game_state = HP;
            break;
        default:
            game_state = BASIC;
    }

    switch (index_size) {
        case 1:
            row = 10;
            col = 10;
            break;
        case 2:
            row = 15;
            col = 15;
            break;
        case 3:
            row = 10;
            col = 20;
            break;
        case 4:
            row = 20;
            col = 20;
            break;
        case 5:
            row = 20;
            col = 30;
            break;
        default:
            row = 3;
            col = 3;
            break;
    }

    switch (index_d) {
        case 1:
            d = 10;
            break;
        case 2:
            d = 50;
            break;
        case 3:
            d = 100;
            break;
        case 4:
            d = 150;
            break;
        case 5:
            d = 200;
            break;
        case 6:
            d = 300;
            break;
        case 7:
            d = 400;
            break;
        case 8:
            d = 500;
            break;
        default:
            d = 1;
            break;
    }
}

void display(int row, int col) {

    if (game_state == HP) {

        cout << "\n";
        cout << "   -PV- " << stats.player_health << "/100";
        cout << "\n";
        cout << "<<< ";
        for (int i{}; i < stats.player_health; i++) {
            cout << "#";
        }
        for (int i{}; i < 100 - stats.player_health; i++) {
            cout << "-";
        }
        cout << " >>>";
        cout << "\n\n";

    }

    cout << "   " << "  ";  // une case vide suivie d'un espace

    for (int i{}; i < col; i++) {  // numéros des colonnes
        if (i < 10) {
            cout << i << "  ";
        } else {
            cout << i << " ";
        }
    }
    cout << "\n";

    cout << "  ";
    for (int i{}; i < col + 1; i++) {  // ligne vide
        cout << "   ";
    }
    cout << "\n";

    for (int i{}; i < row; i++) {
        // numéros des lignes suivis d'un espace
        if (i < 10) {
            cout << i << "  " << "  ";
        } else {
            cout << i << " " << "  ";
        }

        for (int j{}; j < col; j++) {
            cout << "#" << "  ";
        }
        cout << "\n";
    }
}

void display(Matrix *matrix) {

    // barre de vie
    if (game_state == HP) {

        cout << "\n";

        int tmp = stats.player_health < 1 ? 0 : stats.player_health;
        cout << "   -PV- " << tmp << "/100";

        cout << "\n";
        cout << "<<< ";
        for (int i{}; i < stats.player_health; i++) {
            cout << "#";
        }
        for (int i{}; i < 100 - stats.player_health; i++) {
            cout << "-";
        }
        cout << " >>>";
        cout << "\n\n";

    }
    
    // une case vide suivie d'un espace
    cout << "   " << "  ";

    // numéros des colonnes
    for (int i{}; i < matrix->nb_column; i++) {
        if (i < 10) {
            cout << i << "  ";
        } else {
            cout << i << " ";
        }
    }
    cout << "\n";

    // ligne vide
    cout << "  ";
    for (int i{}; i < matrix->nb_column + 1; i++) {
        cout << "   ";
    }
    cout << "\n";

    // Jeu
    for (int i{}; i < matrix->nb_row; i++) {

        // numéros des lignes suivis d'un espace
        if (i < 10) {
            cout << i << "  " << "  ";
        } else {
            cout << i << " " << "  ";
        }

        for (int j = 0; j < matrix->nb_column; j++) {

            switch (matrix->T[i][j]) {
                case BASE:
                    cout << "#  ";
                    break;

                case DUG:
                    if (matrix->M[i][j] == 0) {
                        cout << ".  ";
                    } else {
                        cout << matrix->M[i][j] << "  ";
                    }
                    break;

                case MINE:
                    if (game_state == LOST) {
                        cout << "@  ";
                    } else {
                        cout << "#  ";
                    }
                    break;

                case FLAG:
                    if (game_state == LOST) {

                        if (matrix->M[i][j] == -1) {
                            cout << "@  ";
                        } else {
                            cout << "P  ";
                        }

                    } else {
                        cout << "P  ";
                    }
                    break;

                case BOOM:
                    cout << "@  ";
                    break;
            }
        }
        cout << "\n";
    }

    cout << "\n";
}

void act(Matrix *matrix, coord_pair &p) {

    int row{}, col{};

    cout << "\nOu creuser ?\n";

    cout << "Ligne -> ";
    cin >> row;
    while (row < 0 || row >= matrix->nb_row || !cin) {
        cin.clear();
        cin.ignore();
        cout << "-> ";
        cin >> row;
    }

    cout << "Colonne -> ";
    cin >> col;
    while (col < 0 or col >= matrix->nb_column || !cin) {
        cin.clear();
        cin.ignore();
        cout << "-> ";
        cin >> col;
    }

    p = {row, col};
}

void act(Matrix *matrix, int &a, coord_pair &p) {
    a = 0;

    cout << "\nQue faire ?\n";
    cout << "1. Creuser\n";
    cout << "2. Poser un drapeau\n";
    cout << "3. Lever un drapeau\n";
    cout << "4. Utiliser un joker\n";
    cout << "5. Quitter\n";

    cout << "-> ";
    cin >> a;
    while (a < 1 || a > 5 || !cin) {
        cin.clear();
        cin.ignore();
        cout << "-> ";
        cin >> a;
    }

    if (a == 5) {
        return;
    }

    cout << "\nOu ca ?\n";

    cout << "Ligne -> ";
    cin >> p.row;
    while (p.row < 0 || p.row >= matrix->nb_row || !cin) {
        cin.clear();
        cin.ignore();
        cout << "-> ";
        cin >> p.row;
    }

    cout << "Colonne -> ";
    cin >> p.col;
    while (p.col < 0 or p.col >= matrix->nb_column || !cin) {
        cin.clear();
        cin.ignore();
        cout << "-> ";
        cin >> p.col;
    }

}

void func_clear() {
    std::cout << "\x1B[2J\x1B[3J\x1B[H";
}

void display_banner() {
    func_clear();
    std::vector<std::string> banner;
    // 10 x 98
    banner.push_back(
        " ,gggggggggggg,                                                 "
        "                                  ");
    banner.push_back(
        "dP\"\"\"88\"\"\"\"\"\"Y8b,                                      "
        "                                           ");
    banner.push_back(
        "Yb,  88       `8b,                                              "
        "                                  ");
    banner.push_back(
        " `\"  88        `8b                              gg             "
        "                                   ");
    banner.push_back(
        "     88         Y8                              \"\"            "
        "                                    ");
    banner.push_back(
        "     88         d8  ,ggg,    ,ggg,,ggg,,ggg,    gg    "
        ",ggg,,ggg,    ,ggg,   gg      gg   ,gggggg, ");
    banner.push_back(
        "     88        ,8P i8\" \"8i  ,8\" \"8P\" \"8P\" \"8,   88   "
        ",8\" \"8P\" \"8,  i8\" \"8i  I8      8I   dP\"\"\"\"8I ");
    banner.push_back(
        "     88       ,8P' I8, ,8I  I8   8I   8I   8I   88   I8   8I   "
        "8I  I8, ,8I  I8,    ,8I  ,8'    8I ");
    banner.push_back(
        "     88______,dP'  `YbadP' ,dP   8I   8I   Yb,_,88,_,dP   8I   "
        "Yb, `YbadP' ,d8b,  ,d8b,,dP     Y8,");
    banner.push_back(
        "    888888888P\"   888P\"Y8888P'   8I   8I   `Y88P\"\"Y88P'   "
        "8I   `Y8888P\"Y8888P'\"Y88P\"`Y88P      `Y8");
    
    cout << "\n\n\n";
    for (int i = 0; i < banner.size(); i++) {
        cout << banner[i] << "\n";
    }

    cout << "\n\n";
    cout << "Appuyez sur ENTREE pour continuer...\n";
    cin.get();
}

void rules() {
    func_clear();
    std::vector<std::string> rulebook;

    rulebook.push_back(
        "------ Regles du jeu ------\n"
    );
    rulebook.push_back(
        "Differents symboles du jeu:\n"
        "<.> : une case creuee, sans mines autour.\n"
        "<1> a <8> : une case creusee, avec le nombre de mines autour.\n"
        "<#> : une case qui n'a pas encore ete creusee.\n"
        "<P> : une case avec un drapeau.\n"
        "<@> : une mine.\n"
    );
    rulebook.push_back(
        "Le but du jeu est de creuser toutes les cases qui ne sont pas"
        " des mines.\n"
        "Pour reussir, il faut s'aider des numeros qui s'affichent.\n"
        "Chaque numero indique le nombre de mines dans son entourage"
        " immediat.\n"
        "C'est-a-dire le nombre de mine dans les huit cases autour.\n"
        "Si la case n'est entouree d'aucune mine, elle n'affiche qu'un"
        " point.\n"
        "Lorsqu'une case avec une mine est creusee, la partie et finie.\n"
    );
    rulebook.push_back(
        "L'utilisation d'un joker permet de creuser une case sans risque."
        "\nMais les jokers font diminuer le score.\n"
    );
    rulebook.push_back(
        "Le score est calcule selon trois criteres:\n"
        "\tLe nombre de cases dans le jeu.\n"
        "\tLe nombre de jokers utilises.\n"
        "\tLe temps.\n"
    );
    rulebook.push_back(
        "Il y a deux modes de jeu:\n"
        "-> Mode de jeu basique: le joueur perd des qu'il creuse une case"
        " minee.\n"
        "-> Mode de jeu avec de la vie: le joueur a une barre de vie.\n"
        "Lorsqu'une case minee est creusee, le joueuer perd de la vie.\n"
        "Plus la mine etait facile a trouver, plus le joueur perd de vie."
        "\nLorsque le joueuer perd toute sa vie, la partie est finie.\n"
    );
    
    for (int i = 0; i < rulebook.size(); i++) {
        cout << rulebook[i] << "\n";
    }

    cout << "\n\n";
    cout << "Appuyez sur ENTREE pour continuer...\n";
    cin.clear();
    cin.ignore();
    cin.get();
}
