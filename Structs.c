#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int largeur;
    int hauteur;
    int **map;
} plateau;

typedef struct batiment batiment;
struct batiment
{
    int x;
    int y;
    int hauteur;
    int largeur;
    int ID;
    int prix;
    int revenus;
    int enConstruc;
    int jconst;
    int actif;
    int lvl;
    batiment *suivant;
};

typedef struct player player;
struct player
{
    char name[20];
    int money;
    int gains;
};

typedef struct listeB listeB;
struct listeB
{
    batiment *tete;
    int nbrElements;
};

typedef struct pret pret;
struct pret
{
    int debut;
    int somme;
    int timer;
    pret *suivant;
};

typedef struct listeP listeP;
struct listeP
{
    pret *tete;
    int nbrElements;
};

typedef struct jeu jeu;
struct jeu
{
    //Structs
    plateau *pplateau;
    player *pplayer;
    listeB *plisteB;
    listeP *plisteP;

    //variables
    int days;
    int population;
    int loges;
    int nourriture;
    float natalite;
    float mortalite;
    int dy;
};