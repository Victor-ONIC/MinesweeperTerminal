#pragma once

#include <ctime>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <thread>
#include <string>

struct matrice {
  int nbl;
  int nbc;
  int **T;
};

void destructor_m(matrice *M);
int **creer_tab_2D(int ligne, int colonne);
matrice *construit_matrice(int ligne, int colonne);
void init_mat_zero(matrice &M);

matrice *Init_jeu(int ligne, int colonne, int difficulty,
                  int &first_ligne, int &first_colonne);
matrice *mines_autour(matrice *T);

bool poser_drapeau(matrice *T, int i, int j);
bool lever_drapeau(matrice *T, int i, int j);
int creuser(matrice *T, matrice *M, int i, int j, int &N);
void creuser_recu(matrice *T, matrice *M, int ligne, int colonne, int &N);

int calc_score(matrice *T, int t, bool o);