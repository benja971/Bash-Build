#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

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
		system("cls");
		printf("De quelle taille voulez-vous que votre plateau soit (10 x 10 mininum) ?\n");

		printf("Largeur: ");
		scanf("%d", &(p->largeur));

		printf("Hauteur: ");
		scanf("%d", &(p->hauteur));
	} while (p->largeur < 10 || p->hauteur < 10);
}

liste *insListe()
{
	liste *pl = mallop(sizeof(liste)); // déclaration

	pl->tete = NULL;
	pl->nbrBats = 0;

	return pl;
}

plateau *creerPlateau()
{
	plateau *pp = (plateau *)mallop(sizeof(plateau));

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
			_sleep(1);

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
		pp->largeur = 10;
		pp->hauteur = 10;
		break;

	case 1:
		pp->largeur = 20;
		pp->hauteur = 20;
		break;

	case 2:
		pp->largeur = 20;
		pp->hauteur = 30;
		break;

	case 3:
		pp->largeur = 0;
		pp->hauteur = 0;
		choisirTaille(pp);
		break;
	}

	return pp;
}

void creerTab2d(plateau **ppp)
{
	int **tab = (int **)mallop((*ppp)->largeur * sizeof(int *));

	for (int i = 0; i < (*ppp)->largeur; i++)
		tab[i] = (int *)mallop((*ppp)->hauteur * sizeof(int));

	(*ppp)->map = tab;
}

void refreshTab(plateau *pp, liste *pl)
{
	for (int i = 0; i < pp->largeur; i++)
	{
		for (int j = 0; j < pp->hauteur; j++)
			(pp->map)[i][j] = 0;
	}

	batiment *pb = pl->tete;
	while (pb != NULL)
	{
		dessinerBat(pp, pb, pb->actif ? 2 : 1);
		pb = pb->suivant;
	}
}

void afficherTab(plateau *pp)
{
	printf("\n");

	for (int i = 0; i < pp->largeur; i++)
	{
		for (int j = 0; j < pp->hauteur; j++)
		{
			int mode = (pp->map)[i][j];

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

void dessinerBat(plateau *pp, batiment *pb, int mode)
{
	for (int k = pb->x; k < pb->x + pb->largeur; k++)
	{
		for (int l = pb->y; l < pb->y + pb->hauteur; l++)
			(pp->map)[l][k] = mode;
	}
}

void InsFin(liste *pl, batiment *pb)
{
	if (pl->nbrBats == 0)
		pl->tete = pb;

	else
	{
		batiment *tete = pl->tete;
		while (tete->suivant != NULL)
			tete = tete->suivant;

		tete->suivant = pb;
	}

	pl->nbrBats++;
}

batiment *construireBat(plateau *pp, player *pj, liste *pl, int jour)
{
	batiment *pb = (batiment *)mallop(sizeof(batiment));
	pb->suivant = NULL;
	pb->actif = 0;
	pb->lvl = 0;
	pb->x = 0;
	pb->y = 0;
	pb->jconst = jour;

	InsFin(pl, pb);
	int done = 0, choix = 0;
	while (!done)
	{
		system("cls");

		printf("Type de batiment:\n");
		printf("%s%s\n", choix == 0 ? " > " : "   ", pj->money >= 60 ? "Banque" : "\033[30;1mBanque\033[0m");
		printf("%s%s\n", choix == 1 ? " > " : "   ", pj->money >= 40 ? "Maison" : "\033[30;1mMaison\033[0m");
		printf("%s%s\n", choix == 2 ? " > " : "   ", "Annuler");

		while (!kbhit())
			;

		char key = getch();
		if (key == 'H' && choix > 0)
			choix--;

		if (key == 'P' && choix < 2)
			choix++;

		if (key == '\r' && !(choix == 0 && pj->money < 60) && !(choix == 1 && pj->money < 40))
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
		return NULL;

	done = 0;
	while (!done)
	{
		system("cls");

		printf("position:\n");
		printf("%d, %d\n", pb->x, pb->y);

		refreshTab(pp, pl);
		afficherTab(pp);

		while (!kbhit())
			;

		char key = getch();
		if (key == 'H' && pb->y > 0)
			(pb->y)--;

		if (key == 'P' && pb->y < pp->hauteur - pb->hauteur)
			(pb->y)++;

		if (key == 'K' && pb->x > 0)
			(pb->x)--;

		if (key == 'M' && pb->x < pp->largeur - pb->largeur)
			(pb->x)++;

		if (key == '\r')
		{
			done = 1;
		}
	}
	pb->enConstruc = 1;
	pj->gains += pb->revenus;
	pj->money -= pb->prix;

	return pb;
}

void majBat(liste *pl, int jours)
{
	batiment *cb = pl->tete;
	while (cb)
	{
		if (jours - cb->jconst > 2 && cb->actif == 0)
		{
			cb->enConstruc = 0;
			cb->actif = 1;
			cb->lvl += 1;
		}

		cb = cb->suivant;
	}
}

player *creerPlayer()
{
	player *j = (player *)mallop(sizeof(player));
	printf("Nom du joueur: ");
	fgets(j->name, 19, stdin);
	j->money = 100;
	j->gains = 0;
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

void upgrade(liste *pl, int jours, player *pj)
{
	batiment *cb = pl->tete;
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
			cb->jconst = jours;
			pj->gains -= cb->revenus;
			cb->revenus += 10;
			pj->gains += cb->revenus;
		}
		printf("\n");
		cb = cb->suivant;
	}
}