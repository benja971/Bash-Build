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
    char name[20];
    int prix;
    int revenus;
    // batiment *suivant;
};

typedef struct player player;
struct player
{
    char name[20];
    int money;
    int gains;
};

typedef struct liste_chn liste;
struct liste_chn
{
    batiment *tete;
    int nbrBats;
};
