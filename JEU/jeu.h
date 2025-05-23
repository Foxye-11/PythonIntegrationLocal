#ifndef JEU_H
#define JEU_H

#include "../GRAPHISMES/graphismes.h"
#include "../LAN/LAN.h"
#include "structures.h"
#include "../GRAPHISMES/bouttons.h"

int NB_JOUEURS;

Perso init_player(int num);
void init_game(socket_t sock, Game * game, int num, Perso self);
void jouer(socket_t sock, Game * game, int num);
void init_local_game(Game * game, Perso * liste);
void jouer_local(Game * game);
void init_nb_players();
void jouer_local_graphique(Game * game);
void jouer_graphique(socket_t sock, Game * game, int num);

#endif //JEU_H
