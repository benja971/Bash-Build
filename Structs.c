#include <stdio.h>
#include <stdlib.h>

typedef struct plateau plateau;
struct plateau
{
    int largeur;
    int hauteur;
    int **map;
};

typedef struct batiment batiment;
struct batiment
{
    int x;
    int y;
    int hauteur;
    int largeur;
    char name[20];
    int prix;
    batiment *suivant;
};

typedef struct player player;
struct player
{
    char name[20];
    int money;
};

typedef struct liste_chn liste;
struct liste_chn
{
    batiment *tete;
    int nbrBats;
};
