#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int largeur;
	int hauteur;
	int **map;
} plateau;

typedef struct entite entite;
struct entite
{
	int x;
	int y;
	int hauteur;
	int largeur;
	int ID;
	int capacite;
	int prix;
	int revenusM;
	int revenusN;
	int jconst;
	int actif;
	int lvl;
	entite *suivant;
};

typedef struct player player;
struct player
{
	char name[20];
	int money;
	int gainsM;
	int gainsN;
};

typedef struct listeE listeE;
struct listeE
{
	entite *tete;
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
	listeE *listeEntits;
	listeP *plisteP;

	//variables
	int **terres;
	int days;
	int population;
	int loges;
	int nourriture;
	float natalite;
	float mortalite;
	int dy;
};