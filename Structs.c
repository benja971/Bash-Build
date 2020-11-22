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

typedef struct liste liste;
struct liste
{
    batiment *tete;
    int nbrBats;
};
