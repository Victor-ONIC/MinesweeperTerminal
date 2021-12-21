/**
 * @file main.cpp
 * @author ONIC Victor victor.onic@alumni.univ-avignon.fr
 * @date 13-12-2021
 * 
 * Les cases apparaissent comme ceci: case: # drapeau: P 
 * découvert: '.' ou "n" mines: @.
 */

#include "func.h"
#include "affichage.h"

int main() {

  aff_banner();

  int init_ligne, init_colonne, difficulty;
  menu(init_ligne, init_colonne, difficulty);

  int win_condition = init_ligne * init_colonne - difficulty;

  int action, ligne, colonne;
  matrice *Jeu = Init_jeu(init_ligne, init_colonne, difficulty, ligne, colonne);
  matrice *mine_map = mines_autour(Jeu);

  bool poser_drapeau__flag = false, lever_drapeau__flag = false;
  int creuser__flag = -2;

  int discovered = 0;

  long long int timer_start = time(NULL);

  int win = 0;
  while (win == 0) {
    
    if (discovered == 0) {
      creuser(Jeu, mine_map, ligne, colonne, discovered);
    }

    affichage(Jeu, mine_map, 0);
    
    if (poser_drapeau__flag) {
      std::cout << "\nVous ne pouvez pas poser de drapeau ici.\n";
      poser_drapeau__flag = false;
    }
    if (lever_drapeau__flag) {
       std::cout << "\nVous ne pouvez pas lever de drapeau ici.\n";
       lever_drapeau__flag = false;
    }
    if (creuser__flag == -1) {
       std::cout << "\nVous ne pouvez pas creuser ici.\n";
       creuser__flag = -2;
    }

    do_what(Jeu, action, ligne, colonne, 0);
    switch (action) {
      case 1:
        creuser__flag = creuser(Jeu, mine_map, ligne, colonne, discovered);
        break;
      case 2:
        poser_drapeau__flag = poser_drapeau(Jeu, ligne, colonne);
        break;
      case 3:
        lever_drapeau__flag = lever_drapeau(Jeu, ligne, colonne);
        break;
      case 4:
        std::cout << "\nAu revoir !\n";
        destructor_m(Jeu);
        destructor_m(mine_map);
        return 0;
    }

    if (discovered == win_condition) win = 1;
    if (creuser__flag == 0) win = -1;
  }
  int temps = time(NULL) - timer_start;

  if (win == 1) {  // GAGNÉ
    affichage(Jeu, mine_map, 0);
    std::cout << "\nFELICITATIONS !!!!!!!!!!\n";
    int score = calc_score(Jeu, temps, false);
    std::cout << "\nScore: " << score << std::endl;
  }
  else if (win == -1) {  // PERDU
    affichage(Jeu, mine_map, 1);
    std::cout << "\nDOMMAGE !!!!!!!!!!!!!!!\n";
    int score = calc_score(Jeu, temps, true);
    std::cout << "\nScore: " << score << std::endl;
  }

  destructor_m(Jeu);
  destructor_m(mine_map);
  return 0;
}
