#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define RAND_MAX 0x7FFF

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

void choisirTaille(jeu *pjeu)
{
	do
	{
		system("cls");
		printf("De quelle taille voulez-vous que votre plateau soit (10 x 10 mininum) ?\n");

		printf("Largeur: ");
		scanf("%d", &(pjeu->pplateau->largeur));

		printf("Hauteur: ");
		scanf("%d", &(pjeu->pplateau->largeur));
	} while (pjeu->pplateau->largeur < 10 || pjeu->pplateau->hauteur < 10);
}

listeB *insListeB(jeu *pjeu)
{
	pjeu->plisteB = (listeB *)mallop(sizeof(listeB)); // déclaration

	pjeu->plisteB->tete = NULL;
	pjeu->plisteB->nbrElements = 0;

	return pjeu->plisteB;
}

listeP *insListeP(jeu *pjeu)
{
	pjeu->plisteP = (listeP *)mallop(sizeof(listeP)); // déclaration

	pjeu->plisteP->tete = NULL;
	pjeu->plisteP->nbrElements = 0;

	return pjeu->plisteP;
}

plateau *creerPlateau(jeu *pjeu)
{
	pjeu->pplateau = (plateau *)mallop(sizeof(plateau));

	int done = 0, choix = 0;
	while (!done)
	{
		system("cls");

		printf("Dimensions du plateau:\n");
		printf(choix == 0 ? " > 10 x 10\n" : "   10 x 10\n");
		printf(choix == 1 ? " > 20 x 20\n" : "   20 x 20\n");
		printf(choix == 2 ? " > 20 x 30\n" : "   20 x 30\n");
		printf(choix == 3 ? " > perso\n" : "   perso\n");

		while (!kbhit())
			Sleep(1);

		char key = getch();
		if (key == 'H' && choix > 0)
			choix--;

		if (key == 'P' && choix < 3)
			choix++;

		if (key == '\r')
			done = 1;
	}

	switch (choix)
	{
	case 0:
		pjeu->pplateau->largeur = 10;
		pjeu->pplateau->hauteur = 10;
		break;

	case 1:
		pjeu->pplateau->largeur = 20;
		pjeu->pplateau->hauteur = 20;
		break;

	case 2:
		pjeu->pplateau->largeur = 20;
		pjeu->pplateau->hauteur = 30;
		break;

	case 3:
		pjeu->pplateau->largeur = 0;
		pjeu->pplateau->hauteur = 0;
		choisirTaille(pjeu);
		break;
	}

	return pjeu->pplateau;
}

void creerTab2d(jeu *pjeu)
{
	pjeu->pplateau->map = (int **)mallop((pjeu->pplateau->largeur) * sizeof(int *));

	for (int i = 0; i < pjeu->pplateau->largeur; i++)
		pjeu->pplateau->map[i] = (int *)mallop((pjeu->pplateau->hauteur) * sizeof(int));
}

void dessinerBat(jeu *pjeu, batiment *pb, int mode)
{
	for (int k = pb->x; k < pb->x + pb->largeur; k++)
	{
		for (int l = pb->y; l < pb->y + pb->hauteur; l++)
			(pjeu->pplateau->map)[l][k] = mode;
	}
}

void refreshTab(jeu *pjeu)
{
	for (int i = 0; i < pjeu->pplateau->largeur; i++)
	{
		for (int j = 0; j < pjeu->pplateau->hauteur; j++)
			(pjeu->pplateau->map)[i][j] = 0;
	}

	batiment *pb = pjeu->plisteB->tete;
	while (pb != NULL)
	{
		dessinerBat(pjeu, pb, pb->actif ? 2 : 1);
		pb = pb->suivant;
	}
}

void afficherTab(jeu *pjeu)
{
	printf("\n");
	for (int i = 0; i < pjeu->pplateau->largeur; i++)
	{
		for (int j = 0; j < pjeu->pplateau->hauteur; j++)
		{
			int mode = (pjeu->pplateau->map)[i][j];

			if (mode == 0)
				printf(" \033[30;1m.\033[0m"); // point vide

			if (mode == 1)
				printf(" \033[34;1m#\033[0m"); // batiment à placer

			if (mode == 2)
				printf(" #"); // batiment
		}

		printf("\n");
	}
	printf("\n");
}

void insFinBats(jeu *pjeu, batiment *pb)
{
	if (pjeu->plisteB->nbrElements == 0)
		pjeu->plisteB->tete = pb;

	else
	{
		batiment *tete = pjeu->plisteB->tete;
		while (tete->suivant != NULL)
			tete = tete->suivant;

		tete->suivant = pb;
	}

	pjeu->plisteB->nbrElements++;
}

void insFinPrets(jeu *pjeu, pret *pp)
{
	if (pjeu->plisteP->nbrElements == 0)
		pjeu->plisteP->tete = pp;

	else
	{
		pret *tete = pjeu->plisteP->tete;
		while (tete->suivant != NULL)
			tete = tete->suivant;

		tete->suivant = pp;
	}

	pjeu->plisteP->nbrElements++;
}

batiment *construireBat(jeu *pjeu)
{
	batiment *pb = (batiment *)mallop(sizeof(batiment));
	pb->suivant = NULL;
	pb->actif = 0;
	pb->lvl = 0;
	pb->x = 0;
	pb->y = 0;
	pb->jconst = pjeu->days;

	insFinBats(pjeu, pb);
	int done = 0, choix = 0;
	while (!done)
	{
		system("cls");

		printf("Type de batiment:\n");
		printf("%s%s\n", choix == 0 ? " > " : "   ", pjeu->pplayer->money >= 60 ? "Banque" : "\033[30;1mBanque\033[0m");
		printf("%s%s\n", choix == 1 ? " > " : "   ", pjeu->pplayer->money >= 40 ? "Maison" : "\033[30;1mMaison\033[0m");
		printf("%s%s\n", choix == 2 ? " > " : "   ", pjeu->pplayer->money >= 50 ? "Ferme" : "\033[30;1mFerme\033[0m");
		printf("%s%s\n", choix == 3 ? " > " : "   ", "Annuler");

		while (!kbhit())
			;

		char key = getch();
		if (key == 'H' && choix > 0)
			choix--;

		if (key == 'P' && choix < 3)
			choix++;

		if (key == '\r' && !(choix == 0 && pjeu->pplayer->money < 60) && !(choix == 1 && pjeu->pplayer->money < 40) && !(choix == 2 && pjeu->pplayer->money < 50))
			done = 1;
	}

	if (choix == 0)
	{
		pb->ID = 0;
		pb->prix = 60;
		pb->largeur = 3;
		pb->hauteur = 3;
		pb->revenus = 0;
	}

	if (choix == 1)
	{
		pb->ID = 1;
		pb->prix = 40;
		pb->largeur = 3;
		pb->hauteur = 2;
		pb->revenus = 10;
	}

	if (choix == 2)
	{
		pb->ID = 2;
		pb->prix = 50;
		pb->largeur = 2;
		pb->hauteur = 3;
		pb->revenus = 15;
	}

	if (choix == 3)
		return NULL;

	done = 0;
	while (!done)
	{
		system("cls");

		printf("position:\n");
		printf("%d, %d\n", pb->x, pb->y);

		refreshTab(pjeu);
		afficherTab(pjeu);

		while (!kbhit())
			;

		char key = getch();
		if (key == 'H' && pb->y > 0)
			(pb->y)--;

		if (key == 'P' && pb->y < pjeu->pplateau->hauteur - pb->hauteur)
			(pb->y)++;

		if (key == 'K' && pb->x > 0)
			(pb->x)--;

		if (key == 'M' && pb->x < pjeu->pplateau->largeur - pb->largeur)
			(pb->x)++;

		if (key == '\r')
		{
			done = 1;
		}
	}
	pb->enConstruc = 1;
	pjeu->pplayer->gains += pb->revenus;
	pjeu->pplayer->money -= pb->prix;

	return pb;
}

void newPret(jeu *pjeu)
{
	pret *npret = (pret *)mallop(sizeof(pret));
	npret->debut = pjeu->days;
	npret->somme = 3 * pjeu->pplayer->money;
	npret->timer = 5 * 365;
	npret->suivant = NULL;
	insFinPrets(pjeu, npret);
}

void afficherPrets(jeu *pjeu)
{
	pret *cp = pjeu->plisteP->tete;
	while (cp)
	{
		printf(" / %d jours restants: %de", cp->timer, cp->somme);
		cp = cp->suivant;
	}
}

void majBat(jeu *pjeu)
{
	batiment *cb = pjeu->plisteB->tete;
	while (cb)
	{
		if (pjeu->days - cb->jconst > 2 && cb->actif == 0)
		{
			cb->enConstruc = 0;
			cb->actif = 1;
			cb->lvl += 1;
		}
		cb = cb->suivant;
	}
}

player *creerPlayer(jeu *pjeu)
{
	pjeu->pplayer = (player *)mallop(sizeof(player));
	printf("Nom du joueur: ");
	fgets(pjeu->pplayer->name, 19, stdin);
	pjeu->pplayer->money = 100;
	pjeu->pplayer->gains = 0;
}

void afficherBat(batiment *pb)
{
	if (pb->ID == 0)
		printf("%s\n", "Banque");

	else
		printf("%s\n", "Maison");

	printf("niveau: %d\n", pb->lvl);
	printf("revenus: %d\n", pb->revenus);
}

void upgrade(jeu *pjeu)
{
	if (pjeu->plisteB->nbrElements > 0)
	{
		batiment *cb = pjeu->plisteB->tete;
		char choix;

		while (cb)
		{
			afficherBat(cb);
			printf("Voulez vous améliorer ?(o/n)");
			scanf("%c", &choix);
			viderBuffer();
			if (choix == 'o')
			{
				cb->actif = 0;
				cb->enConstruc = 1;
				cb->jconst = pjeu->days;
				pjeu->pplayer->gains -= cb->revenus;
				cb->revenus += 10;
				pjeu->pplayer->gains += cb->revenus;
			}
			printf("\n");
			cb = cb->suivant;
		}
	}
	else
	{
		printf("Pas de batiments a ameliorer.");
	}
}

void timerPrets(jeu *pjeu)
{
	pret *cp = pjeu->plisteP->tete;
	while (cp)
	{
		cp->timer -= 1;

		if (cp->timer == 0)
			pjeu->pplayer->money -= cp->somme;

		cp = cp->suivant;
	}
}

void retirerDebut(jeu *pjeu)
{
	pret *np = pjeu->plisteP->tete;
	pjeu->plisteP->tete = pjeu->plisteP->tete->suivant;
	free(np);
}

void rembourserPret(jeu *pjeu)
{
	if (pjeu->pplayer->money > pjeu->plisteP->tete->somme * 1.05 && pjeu->plisteP->nbrElements > 0)
	{
		pjeu->pplayer->money -= pjeu->plisteP->tete->somme * 1.05;
		retirerDebut(pjeu);
	}
}

float random()
{
	return (float)(rand() % 100000) / 100000.0;
}

void ev_Pop(jeu *pjeu)
{
	for (int i = 0; i < pjeu->population; i++)
	{
		if (random() < pjeu->natalite)
			pjeu->population += 1;

		if (random() < pjeu->mortalite)
			pjeu->population -= 1;
	}
}