#include "func.h"

#include "affichage.h"

/// Supprime une matrice allouée dynamiquement.
void destructor_m(matrice *M) {
    for (int supp = 0; supp < M->nbl; supp++) {
        delete[] M->T[supp];
    }
    delete[] M->T;
    delete M;
}

/// Alloue un tableau 2D.
int **creer_tab_2D(int ligne, int colonne) {
    int **T = new int *[ligne];
    for (int i = 0; i < ligne; i++) {
        T[i] = new int[colonne];
    }
    return T;
}

/// Alloue une matrice.
matrice *construit_matrice(int ligne, int colonne) {
    matrice *m = new matrice;
    m->nbl = ligne;
    m->nbc = colonne;
    m->T = creer_tab_2D(ligne, colonne);
    return m;
}

/// Remplis le tableau d'une matrice avec des zéros.
void init_mat_zero(matrice &M) {
    for (int i = 0; i < M.nbl; i++) {
        for (int j = 0; j < M.nbc; j++) {
            M.T[i][j] = 0;
        }
    }
}

bool condition_init_jeu(matrice *T, int mine_ligne, int mine_colonne,
                        int ask_ligne, int ask_colonne) {
    return (
        mine_ligne < 0 || mine_colonne < 0 ||
        T->T[mine_ligne][mine_colonne] == 2 ||
        ((mine_ligne >= ask_ligne - 1 && 
          mine_ligne <= ask_ligne + 1) &&
         (mine_colonne >= ask_colonne - 1 &&
          mine_colonne <= ask_colonne + 1)));
}

/**
 * Initialise la matrice du jeu avec des mines aléatoires après la
 * première creusée.
 *
 * @param ligne,colonne dimensions du jeu.
 * @param difficulty nombre de mines.
 * @param[out] first_ligne,first_colonne position de la première creusée.
 */
matrice *Init_jeu(int nb_ligne, int nb_colonne, int difficulty,
                  int &first_ligne, int &first_colonne) {
    srand(time(NULL));

    matrice *T = construit_matrice(nb_ligne, nb_colonne);
    init_mat_zero((*T));

    affichage(T, T, 0);

    int dummy, ask_ligne, ask_colonne;
    do_what(T, dummy, ask_ligne, ask_colonne, 1);
    first_ligne = ask_ligne;
    first_colonne = ask_colonne;

    for (int i = 0; i < difficulty; i++) {  // placer les mines
        int mine_ligne = -1, mine_colonne = -1;
        while (condition_init_jeu(T, mine_ligne, mine_colonne, ask_ligne,
                                  ask_colonne)) {
            mine_ligne = rand() % nb_ligne;
            mine_colonne = rand() % nb_colonne;
        }
        T->T[mine_ligne][mine_colonne] = 2;
    }

    return T;
}

/**
 * Alloue une matrice contenant le nombre de mines autour de chaque case
 * de la matrice passée en paramètre.
 */
matrice *mines_autour(matrice *T) {
    matrice *M = construit_matrice(T->nbl, T->nbc);
    init_mat_zero((*M));

    for (int ligne = 0; ligne < M->nbl; ligne++) {
        for (int colonne = 0; colonne < M->nbc; colonne++) {
            if (T->T[ligne][colonne] != 2) {
                // Si ligne = 0 -> tout en haut (is_lig_zero)
                // Si colonne = 0 -> tout à gauche (is_col_zero)
                // Si ligne = M->nbl - 1 -> tout en bas (is_lig_max)
                // Si colonne = M->nbc - 1 -> tout à droite (is_col_max)
                bool is_lig_zero = ligne == 0 ? 1 : 0;
                bool is_col_zero = colonne == 0 ? 1 : 0;
                bool is_lig_max = ligne == (M->nbl - 1) ? 1 : 0;
                bool is_col_max = colonne == (M->nbc - 1) ? 1 : 0;
                for (int i = (ligne - 1 + is_lig_zero);
                     i <= (ligne + 1 - is_lig_max); i++) {
                    for (int j = (colonne - 1 + is_col_zero);
                         j <= (colonne + 1 - is_col_max); j++) {
                        if (i == ligne && j == colonne) continue;
                        if (T->T[i][j] == 2) M->T[ligne][colonne]++;
                    }
                }
            } else {  // si c'est une mine
                M->T[ligne][colonne] = -1;
            }
        }
    }
    return M;
}

/**
 * Actualise la case en rajoutant un drapeau.
 *
 * @param *T matrice du jeu.
 * @param i,j position de la case à drapeauter.
 *
 * @returns false si le drapeau a pu être placé, true sinon.
 */
bool poser_drapeau(matrice *T, int i, int j) {
    if (T->T[i][j] == 0) {
        T->T[i][j] = 3;
        return false;
    }
    if (T->T[i][j] == 2) {
        T->T[i][j] = 4;
        return false;
    }
    return true;
}

/**
 * Actualise la case en retirant un drapeau.
 *
 * @param *T matrice du jeu.
 * @param i,j position de la case avec un drapeau.
 *
 * @returns false si le drapeau a pu être retiré, true sinon.
 */
bool lever_drapeau(matrice *T, int i, int j) {
    if (T->T[i][j] == 3) {
        T->T[i][j] = 0;
        return false;
    }
    if (T->T[i][j] == 4) {
        T->T[i][j] = 2;
        return false;
    }
    return true;
}

/**
 * Creuse une case, puis toutes les cases adjacentes qui ne contiennement
 * pas de mines. Utilisée en complément avec creuser_recu.
 *
 * @param T matrice du jeu.
 * @param M matrice des mines.
 * @param i,j position de la case à creuser.
 * @param N nombre de cases découvertes.
 *
 * @returns -1 si la case ne peut pas être creusée.
 *          0 si la case creusée contient une mine.
 *          1 si la case creusée est vide.
 */
int creuser(matrice *T, matrice *M, int i, int j, int &N) {
    if (!(T->T[i][j] == 0 ||
          T->T[i][j] == 2)) {  // ne peut pas être creusée
        return -1;
    }
    if (T->T[i][j] == 2) {  // creuse une mine
        return 0;
    }

    T->T[i][j] = 1;  // creuse
    N++;
    creuser_recu(T, M, i, j, N);
    return 1;
}
void creuser_recu(matrice *T, matrice *M, int ligne, int colonne,
                  int &N) {
    if (M->T[ligne][colonne] == 0) {
        bool is_lig_zero = ligne == 0 ? 1 : 0;
        bool is_col_zero = colonne == 0 ? 1 : 0;
        bool is_lig_max = ligne == (T->nbl - 1) ? 1 : 0;
        bool is_col_max = colonne == (T->nbc - 1) ? 1 : 0;

        for (int m = (ligne - 1 + is_lig_zero);
             m <= (ligne + 1 - is_lig_max); m++) {
            for (int n = (colonne - 1 + is_col_zero);
                 n <= (colonne + 1 - is_col_max); n++) {
                if (T->T[m][n] == 1) continue;
                creuser(T, M, m, n, N);
            }
        }
    }
}

/**
 * Calcule le score de la partie.
 *
 * @param T matrice du jeu.
 * @param t durée de la partie.
 * @param o indique si la partie était gagnante ou perdante.
 */
int calc_score(matrice *T, int t, bool o) {
    int s = 0;
    int N = 0, D = 0;
    for (int i = 0; i < T->nbl; i++) {
        for (int j = 0; j < T->nbc; j++) {
            if (T->T[i][j] == 1) N += 500;
            if (T->T[i][j] == 4) D += 1000;
        }
    }
    if (o) {
        s = 200 * ((N + D) / (50 * t));
    } else {
        s = 200 * ((N + D) / (20 * t));
    }
    return s;
}
