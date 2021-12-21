#pragma once

#include <iostream>

#include "func.h"

using namespace std;

void func_clear(void);

void menu(int &ligne, int &colonne, int &difficulty);
void do_what(matrice *T, int &action, int &i, int &j, bool is_init);

void affichage(matrice *T, matrice *M, int lost);
void aff_banner(void);
