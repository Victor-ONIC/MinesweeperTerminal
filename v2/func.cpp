#include "func.h"
#include "display.h"

static void init_mines(Matrix* matrix, int d, coord_pair p);
static bool surroundings(int row, int col, coord_pair p);
static void mines_around(Matrix* matrix);
static void mines_around_cell(Matrix *matrix, int row, int col);

static int damage(Matrix *matrix, coord_pair p);

static State **create_2D_array_state(int row, int column);
static int **create_2D_array_int(int row, int column);
static Matrix *create_matrix(int row, int column);
static void init_matrix_zero(Matrix *matrix);

extern Game game_state;
extern player_stats stats;


Matrix *init_game(int &difficulty) {
    int row{}, col{}, d{};

    // afficher la bannière.
    display_banner();

    // afficher le menu -> demander taille et difficulté.
    menu(row, col, d);

    // créer la matrice Jeu et initialiser les tableaux à 0.
    Matrix *J = create_matrix(row, col);
    init_matrix_zero(J);

    // afficher un tableau placeholder (mines pas encore générées).
    func_clear();
    display(row, col);

    // demander au joueur où creuser.
    coord_pair p{};
    act(J, p);

    // placer les mines.
    init_mines(J, d, p);
    mines_around(J);

    // creuser la première case.
    dig(J, p, false);

    difficulty = d;
    return J;
}

void init_mines(Matrix* matrix, int d, coord_pair p) {
    // mettre des mines random sauf autour de p.
    srand(time(NULL));

    for (int i{}; i < d; i++) {
        int r_row{}, r_col{};
        while (matrix->T[r_row][r_col] == State::MINE ||
               surroundings(r_row, r_col, p)) {
            r_row = rand() % matrix->nb_row;
            r_col = rand() % matrix->nb_column;
        }
        matrix->T[r_row][r_col] = State::MINE;
    }
}

bool surroundings(int row, int col, coord_pair p) {
    bool condition = (
        (row >= p.row - 1) && (row <= p.row + 1) &&
        (col >= p.col - 1) && (col <= p.col + 1)
    );
    return condition;
}

void mines_around(Matrix* matrix) {
    for (int i{}; i < matrix->nb_row; i++) {
        for (int j{}; j < matrix->nb_column; j++) {

            if (matrix->T[i][j] == State::MINE) {
                matrix->M[i][j] = -1;
            } else {
                mines_around_cell(matrix, i, j);
            }

        }
    }
}

void mines_around_cell(Matrix *matrix, int row, int col) {

    bool is_top = row == 0 ? 1 : 0;
    bool is_bottom = row == (matrix->nb_row - 1) ? 1 : 0;

    bool is_left = col == 0 ? 1 : 0;
    bool is_right = col == (matrix->nb_column - 1) ? 1 : 0;

    int i = row - 1 + is_top;
    for (i; i <= (row + 1 - is_bottom); i++) {
        int j = col - 1 + is_left;
        for (j; j <= (col + 1 - is_right); j++) {

            if (i == row && j == col) {
                continue;
            }

            if (matrix->T[i][j] == State::MINE) {
                matrix->M[row][col]++;
            }

        }
    }
}

void matrix_destructor(Matrix *matrix) {
    for (int s{}; s < matrix->nb_row; s++) {
        delete[] matrix->T[s];
    }
    delete[] matrix->T;

    for (int s{}; s < matrix->nb_row; s++) {
        delete[] matrix->M[s];
    }
    delete[] matrix->M;
    
    delete matrix;
}

State **create_2D_array_state(int row, int column) {
    State **T = new State *[row];
    for (int i = 0; i < row; i++) {
        T[i] = new State[column];
    }
    return T;
}

int **create_2D_array_int(int row, int column) {
    int **T = new int *[row];
    for (int i = 0; i < row; i++) {
        T[i] = new int[column];
    }
    return T;
}

Matrix *create_matrix(int row, int column) {
    Matrix *matrix = new Matrix;
    matrix->nb_row = row;
    matrix->nb_column = column;
    matrix->T = create_2D_array_state(row, column);
    matrix->M = create_2D_array_int(row, column);
    return matrix;
}

void init_matrix_zero(Matrix *matrix) {
    for (int i{}; i < matrix->nb_row; i++) {
        for (int j{}; j < matrix->nb_column; j++) {
            matrix->T[i][j] = State::BASE;
        }
    }

    for (int i{}; i < matrix->nb_row; i++) {
        for (int j{}; j < matrix->nb_column; j++) {
            matrix->M[i][j] = 0;
        }
    }
}

void dig(Matrix *matrix, coord_pair p, bool joker) {
    using std::cout;
    using std::cin;

    State cell = matrix->T[p.row][p.col];

    if (!(cell == State::BASE || cell == State::MINE)) {
        cout << "Cette case ne peut pas etre creusee.\n";
        cin.clear();
        cin.ignore();
        cin.get();
        return;
    }

    if (cell == State::MINE) {
        if (joker) {
            matrix->T[p.row][p.col] = State::BOOM;
            return;
        }
        if (game_state == Game::HP) {
            matrix->T[p.row][p.col] = State::BOOM;

            int damage_amount = damage(matrix, p);
            stats.player_health -= damage_amount;

            return;
        } else {
            game_state = Game::LOST;
            return;
        }
    }

    matrix->T[p.row][p.col] = State::DUG;
    stats.discovered++;

    // Si la case creusée n'a pas de mines autour, alors creuser toutes 
    // les cases adjacentes.
    if (matrix->M[p.row][p.col] == 0) {

        bool is_top = p.row == 0 ? 1 : 0;
        bool is_bottom = p.row == (matrix->nb_row - 1) ? 1 : 0;

        bool is_left = p.col == 0 ? 1 : 0;
        bool is_right = p.col == (matrix->nb_column - 1) ? 1 : 0;

        int i = p.row - 1 + is_top;
        for (i; i <= (p.row + 1 - is_bottom); i++) {
            int j = p.col - 1 + is_left;
            for (j; j <= (p.col + 1 - is_right); j++) {
                
                if (matrix->T[i][j] == State::DUG) {
                    continue;
                }
                
                coord_pair next{i, j};
                dig(matrix, next, false);
            }
        }

    }

}

int damage(Matrix *matrix, coord_pair p) {
    // si tu te prends une mine facile, tu perds beaucoup.
    // mais si c'est une mine difficile à trouver, tu perds moins.
    int damage{}, index_ones{};

    bool is_top = p.row == 0 ? 1 : 0;
    bool is_bottom = p.row == (matrix->nb_row - 1) ? 1 : 0;

    bool is_left = p.col == 0 ? 1 : 0;
    bool is_right = p.col == (matrix->nb_column - 1) ? 1 : 0;

    int i = p.row - 1 + is_top;
    for (i; i <= (p.row + 1 - is_bottom); i++) {
        int j = p.col - 1 + is_left;
        for (j; j <= (p.col + 1 - is_right); j++) {
            
            if (matrix->M[i][j] == -1) {
                damage++;
            } else if (matrix->M[i][j] == 1) {
                damage += 5 + index_ones;
                index_ones++;
            } else {
                damage += matrix->M[i][j];
            }

        }
    }

    return damage;
}

void flag_down(Matrix *matrix, coord_pair p) {
    using std::cout;
    using std::cin;

    State cell = matrix->T[p.row][p.col];

    if (!(cell == State::BASE || cell == State::MINE)) {
        cout << "Vous ne pouvez pas poser de drapeau ici.\n";
        cin.clear();
        cin.ignore();
        cin.get();
        return;
    }

    matrix->T[p.row][p.col] = State::FLAG;
}

void flag_up(Matrix *matrix, coord_pair p) {
    using std::cout;
    using std::cin;

    State cell = matrix->T[p.row][p.col];

    if (cell != State::FLAG) {
        cout << "Il n'y a pas de drapeau ici.\n";
        cin.clear();
        cin.ignore();
        cin.get();
        return;
    }

    matrix->T[p.row][p.col] = State::BASE;
}

int score(int t) {
    // le score augmente quand le joueuer fait un truc qui avance le jeu.
    // le score diminue avec le temps.
    // le score diminue beaucoup avec les jokers, coorects ou pas.

    int score = {
        stats.discovered * 100
        - t * 25
        - stats.jokers * 500
    };

    return score;
}
