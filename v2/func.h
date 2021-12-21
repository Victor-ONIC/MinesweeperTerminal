#pragma once

#include <cstdlib>
#include <ctime>


enum Game { LOST, BASIC, HP, WON };
enum State { BASE, DUG, MINE, FLAG, BOOM };

struct Matrix {
    int nb_row;
    int nb_column;
    State **T;
    int **M;
};

struct coord_pair {
    int row;
    int col;
};

struct player_stats {
    int discovered;
    int jokers;
    int player_health;
};

Matrix *init_game(int &difficulty);
void matrix_destructor(Matrix *matrix);

void dig(Matrix *matrix, coord_pair p, bool joker);
void flag_down(Matrix *matrix, coord_pair p);
void flag_up(Matrix *matrix, coord_pair p);

int score(int t);