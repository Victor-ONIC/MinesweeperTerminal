#include "affichage.h"

/**
 * Efface le terminal afin d'afficher le jeu. Fonctionne partout sauf sur 
 * l'invite de commande Windows.
 */
void func_clear(void) {
  cout << "\x1B[2J\x1B[3J\x1B[H";
}

/**
 * Affiche une interface afin que le joueur choisisse les caractéristiques du
 * jeu.
 * 
 * @param[out] ligne hauteur du jeu.
 * @param[out] colonne largeur du jeu.
 * @param[out] difficulty nombre de mines dans le jeu.
 */
void menu(int &ligne, int &colonne, int &difficulty) {  // OK
  int taille = 0;
  int choix_diff = 0;
  while ((taille == 1 && choix_diff > 2) || (taille == 2 && choix_diff > 6) ||
         taille <= 0 || taille > 4 || choix_diff <= 0 || choix_diff > 8) {
    func_clear();
    cout << "\n\n\n\n\nChoisissez une taille de tableau: \n";  // max 30 x 50
    cout << "1.           10 x 10" << endl;
    cout << "2.           20 x 20" << endl;
    cout << "3.           30 x 30" << endl;
    cout << "4.           30 x 50" << endl;
    cout << endl;
    cin >> taille;

    cout << "\nChoisissez la difficulte du jeu: \n";
    cout << "1.           10 mines" << endl;
    cout << "2.           50 mines" << endl;
    cout << "3.           100 mines" << endl;
    cout << "4.           150 mines" << endl;
    cout << "5.           200 mines" << endl;
    cout << "6.           300 mines" << endl;
    cout << "7.           400 mines" << endl;
    cout << "8.           500 mines" << endl;
    cout << endl;
    cin >> choix_diff;
  }

  switch (taille) {
    case 1:  
      ligne = 10;
      colonne = 10;
      break;
    case 2:  
      ligne = 20;
      colonne = 20;
      break;
    case 3:  
      ligne = 30;
      colonne = 30;
      break;
    case 4:  
      ligne = 30;
      colonne = 50;
      break;
    default:
      ligne = 3;
      colonne = 3;
      break;
  }
  switch (choix_diff) {
    case 1:
      difficulty = 10;
      break;
    case 2:
      difficulty = 50;
      break;
    case 3:
      difficulty = 100;
      break;
    case 4:
      difficulty = 150;
      break;
    case 5:
      difficulty = 200;
      break;
    case 6:
      difficulty = 300;
      break;
    case 7:
      difficulty = 400;
      break;
    case 8:
      difficulty = 500;
      break;
    default:
      difficulty = 1;
      break;
  }
}

/**
 * Demande à l'utilisateur quelle action faire et où.
 * 
 * @param T matrice du jeu.
 * @param[out] action action à réaliser.
 * @param[out] i,j position de la case où effectuer l'action.
 * @param is_init indique si c'est l'appel initial de la fonction.
 */
void do_what(matrice *T, int &action, int &i, int &j, bool is_init) {
  action = 0, i = -1, j = -1;

  if (is_init) {
    cout << "\nOu creuser ? ( par exemple -> 4 (ligne 4), -> 6 (colonne 6) )";
    do {
      cout << endl;
      cout << "-> ";
      cin >> i;
      cout << "-> ";
      cin >> j;
    } while (i < 0 || j < 0 || i >= T->nbl || j >= T->nbc);
    return;
  }

  cout << "\nQue faire ?\n";
  cout << "1. Creuser\n";
  cout << "2. Poser un drapeau\n";
  cout << "3. Lever un drapeau\n";
  cout << "4. Quitter";
  do {
    cout << endl;
    cout << "-> ";
    cin >> action;
  } while (action <= 0 || action > 4);

  if (action == 4) return;

  cout << "\nOu ?";
  do {
    cout << endl;
    cout << "-> ";
    cin >> i;
    cout << "-> ";
    cin >> j;
  } while (i < 0 || j < 0 || i >= T->nbl || j >= T->nbc);
}

/**
 * Affiche le jeu.
 * 
 * @param T matrice du jeu.
 * @param M matrice des mines.
 * @param lost indique s'il faut afficher les mines quand le joueuer a perdu.
 */
void affichage(matrice *T, matrice *M, int lost) {
  func_clear();
  cout << "  " << "   ";
  for (int aff_col = 0; aff_col < T->nbc; aff_col++) {
    if (aff_col < 10) cout << aff_col << "  ";
    else cout << aff_col << " ";
  }
  cout << endl;
  for (int s_h = 0; s_h < T->nbc + 2; s_h++) {  // séparateur horizontal
    cout << "   ";
  }
  cout << endl;

  for (int i = 0; i < T->nbl; i++) {
    if (i < 10) cout << i << "  " << "  ";
    else cout << i << " " << "  ";  // séparateur vertical
    for (int j = 0; j < T->nbc; j++) {
    // erreur:? case:# drapeau:P découvert:'.'ou"n" mines:@
      if (T->T[i][j] == 0) {
        cout << "#" << "  ";
      }
      else if (T->T[i][j] == 1) {
        if (M->T[i][j] == 0) cout << "." << "  ";
        else cout << M->T[i][j] << "  ";
      }
      else if (T->T[i][j] == 2) {
        if (lost == 1) cout << "@" << "  ";
        else cout << "#" << "  ";
      }
      else if (T->T[i][j] == 3) {
        cout << "P" << "  ";
      }
      else if (T->T[i][j] == 4) {
        if (lost == 1) cout << "@" << "  ";
        else cout << "P" << "  ";
      }
      else {
        cout << "?" << "  ";  // Erreur
      }
    }
    cout << endl;
  }
}

/// Affiche la bannière du jeu.
void aff_banner(void) {
  func_clear();
  vector<string> banner;
  // 10 x 98
  banner.push_back(" ,gggggggggggg,                                                                                   ");
  banner.push_back("dP\"\"\"88\"\"\"\"\"\"Y8b,                                                                                 ");
  banner.push_back("Yb,  88       `8b,                                                                                ");
  banner.push_back(" `\"  88        `8b                              gg                                                ");
  banner.push_back("     88         Y8                              \"\"                                                ");
  banner.push_back("     88         d8  ,ggg,    ,ggg,,ggg,,ggg,    gg    ,ggg,,ggg,    ,ggg,   gg      gg   ,gggggg, ");
  banner.push_back("     88        ,8P i8\" \"8i  ,8\" \"8P\" \"8P\" \"8,   88   ,8\" \"8P\" \"8,  i8\" \"8i  I8      8I   dP\"\"\"\"8I ");
  banner.push_back("     88       ,8P' I8, ,8I  I8   8I   8I   8I   88   I8   8I   8I  I8, ,8I  I8,    ,8I  ,8'    8I ");
  banner.push_back("     88______,dP'  `YbadP' ,dP   8I   8I   Yb,_,88,_,dP   8I   Yb, `YbadP' ,d8b,  ,d8b,,dP     Y8,");
  banner.push_back("    888888888P\"   888P\"Y8888P'   8I   8I   `Y88P\"\"Y88P'   8I   `Y8888P\"Y8888P'\"Y88P\"`Y88P      `Y8");
  for (int i = 0; i < banner.size(); i++) cout << banner[i] << endl;

  cout << endl << endl;

  cout << "Press ENTER to continue.\n";
  cin.get();
}
