#ifndef STRUCTURES_H
#define STRUCTURES_H

#define PLAT_X 20
#define PLAT_Y 20

#include <allegro.h>

#define TILE_COUNT 4
#define MAX_NODE PLAT_X*PLAT_Y
typedef struct {
    char* nom_competence;
    int ID_competence;
    int degat;
    char type_degat; //C(contandant), T(tranchant), P(percant), E(eau), F(feu), S(sol/terre), N(neutre/statut)
    char type_stat; //F(faith), S(strength), I(intelligence), D(dexterity)
    int portee;
    int p_attaque;
    BITMAP* sprite[3]; //3 frames par compétences
}t_competence;

typedef struct {
    t_competence competences[4];
    int pv;
    int mana;
    int endurance;
    int force;
    int dexterite;
    int intelligence;
    int foi;
    float r_contandant;
    float r_tranchant;
    float r_percant;
    float r_eau;
    float r_feu;
    float r_terre;
    BITMAP* sprite[8]; //4 sprites de déplacement et un de menu
}t_classev1;

typedef struct {
    t_competence competences[4];
    int statistique[7]; //pv, mana, endurance, force, dexterite, intelligence, foie
    int resistance[6]; //contandant, tranchant, percant, eau, feu, terre
    BITMAP* sprite[5]; //4 sprites de déplacement et un de menu
}t_classev2;

typedef struct {
    int x;
    int y;
    int precednt_x;
    int precedent_y;
}Node;

typedef struct {
    //Position
    int tile_x;
    int tile_y;
    int pixel_x;
    int pixel_y;
    int x;
    int y;
    //Caractéristiques
    char* pseudo;
    t_classev1 classev1;
    t_classev2 classev2;

    int pm_max; //Stat d'endurance
    int pm_restants;
    int p_attaque; //Stat de mana
    int pv_actuels; //Stat de pv * 10
} Joueur;


typedef struct {
    BITMAP* images[TILE_COUNT];
    BITMAP* background;
} Map;


typedef struct {
    // partie imuable ( envoyée dans initGame )
    char pseudo[20];
    char avatar[2];

    // pertie changeante ( envoyée à chaque tour )
    int x,y;

}Perso;

typedef struct {
    char name[20];
} Client;

typedef struct {
    int plateau[PLAT_X][PLAT_Y];
    Perso players[4];
    Client client;
    Map map;
}Game;


#endif //STRUCTURES_H
