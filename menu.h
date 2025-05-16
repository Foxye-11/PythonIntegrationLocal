#ifndef MENU_H
#define MENU_H

#include "JEU/structures.h"

void menu(int * choix );
void init_nb_players_graphique();
Perso init_player_graphique(int num);
void menu_waiting();
void menu_afficher_image(const char* chemin_image);
void menu_selec_perso(Perso* self);

#endif //MENU_H
