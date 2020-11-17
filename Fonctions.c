#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *mallop(size_t size)
{
	void *p = NULL;
	p = malloc(size);

	if (p == NULL)
	{
		printf("Erreur de memoire.\n");
		exit(0);
	}

	return p;
}

void viderBuffer()
{
	int c = 0;
	while (c != '\n' && c != EOF)
		c = getchar();
}

void choisirTaille(plateau *p)
{
	do
	{
		printf("De quelle taille voulez-vous que votre plateau soit (10 x 10 mininum) ?\n");
		printf("Largeur: ");
		scanf("%d", &(p->largeur));
		printf("Hauteur: ");
		scanf("%d", &(p->hauteur));
	} while (p->largeur < 10 || p->hauteur < 10);
}

liste *insListe()
{
	liste *liste = mallop(sizeof(liste)); // déclaration

	liste->tete = NULL;
	liste->nbrBats = 0;

	return liste;
}

plateau *creerPlateau()
{
	int choix;
	plateau *p = (plateau *)mallop(sizeof(plateau));
	printf("Dimensions du tableau de jeu possibles:\n");
	printf("\t 1: 10 x 10\n");
	printf("\t 2: 20 x 20\n");
	printf("\t 3: 20 x 30\n");
	printf("Quelle dimension voulez vous (1, 2, 3 ou 0 pour choisir votre taille) ?\nChoix : ");
	scanf("%d", &choix);

	switch (choix)
	{
	case 0:
		choisirTaille(p);
		break;

	case 1:
		printf("alive");
		p->largeur = 10;
		p->hauteur = 10;
		break;

	case 2:
		p->largeur = 20;
		p->hauteur = 20;
		printf("alive");
		break;

	case 3:
		p->largeur = 20;
		p->hauteur = 40;
		printf("alive");
		break;
	}
	return p;
}

void creerTab2d(plateau **p)
{
	int **tab = (int **)mallop((*p)->largeur * sizeof(int *));

	for (int i = 0; i < (*p)->largeur; i++)
		tab[i] = (int *)mallop((*p)->hauteur * sizeof(int));

	(*p)->map = tab;
}

void initTab(plateau **p)
{
	for (int i = 0; i < (*p)->largeur; i++)
	{
		for (int j = 0; j < (*p)->hauteur; j++)
			((*p)->map)[i][j] = 0;
	}
}

void afficherTab(plateau *p)
{
	printf("\n");

	for (int i = 0; i < p->largeur; i++)
	{
		for (int j = 0; j < p->hauteur; j++)
		{
			int b = (p->map)[i][j];
			if (b)
				printf("%3d", b);
			else
				printf("  .");
		}

		printf("\n");
	}

	printf("\n");
}

void acheterBatiment(batiment *b, player *p)
{
	p->money -= b->prix;
}

void ajouterBat(plateau **p, batiment *b)
{
	for (int i = 0; i < (*p)->largeur; i++)
	{
		for (int j = 0; j < (*p)->hauteur; j++)
		{

			if (b->x == i && b->y == j)
			{
				for (int k = i; k < i + b->largeur; k++)
				{
					for (int l = j; l < j + b->hauteur; l++)
						((*p)->map)[l][k] = 1;
				}
			}
		}
	}
}

// void InsFin(liste *liste, plateau *p, player *j, batiment *cb)
// {
// 	if (liste->nbrBats == 0)
// 		liste->tete = cb;

// 	else
// 	{
// 		batiment *bb = liste->tete;
// 		while (bb->suivant != NULL) // parcours de la liste
// 			bb = bb->suivant;

// 		bb->suivant = bb;
// 	}

// 	liste->nbrBats += 1;
// }

batiment *creerBatiment(plateau *p, player *j, liste *liste)
{
	batiment *b = (batiment *)mallop(sizeof(batiment));

	b->largeur = 3;
	b->hauteur = 2;

	viderBuffer();
	printf("Type de batiment: ");
	fgets(b->name, 19, stdin);

	printf("Position x du batiment: ");
	scanf("%d", &b->x);

	printf("Position y du batiment: ");
	scanf("%d", &b->y);

	if (strcmp(b->name, "Banque") == 1 || strcmp(b->name, "banque") == 1)
	{
		b->prix = 60;
		b->largeur = 3;
		b->hauteur = 3;
		b->revenus = 0;
	}

	if (strcmp(b->name, "Maison") == 1 || strcmp(b->name, "maison") == 1)
	{
		b->prix = 40;
		b->largeur = 3;
		b->hauteur = 2;
		b->revenus = 10;
	}

	// b->suivant = NULL;

	if (b->x + b->largeur > p->largeur || b->y + b->hauteur > p->hauteur)
		printf("\nImpossible de construire ce batiment ici.\n");

	else if (b->prix > j->money)
		printf("\nVous n'avez pas suffisement d'argent pour ca.\n");

	else if (b->prix < j->money)
	{
		acheterBatiment(b, j);
		// InsFin(liste, p, j, b);
		ajouterBat(&p, b);
		j->gains += b->revenus;
	}

	return b;
}

player *creerPlayer()
{
	player *j = (player *)mallop(sizeof(player));
	printf("Quel est votre nom: ");
	fgets(j->name, 19, stdin);
	j->money = 100;
	j->gains = 0;
}