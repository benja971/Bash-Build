#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define SHOW_MAP_NUM 0

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
	pjeu->pplateau->largeur = 0;
	pjeu->pplateau->hauteur = 0;

	while (pjeu->pplateau->largeur <= 1 || pjeu->pplateau->hauteur <= 1)
	{
		system("cls");
		printf("De quelle taille voulez-vous que votre plateau soit (1 x 1 mininum) ?\n");

		printf("Largeur: ");
		scanf("%d", &(pjeu->pplateau->largeur));

		printf("Hauteur: ");
		scanf("%d", &(pjeu->pplateau->hauteur));
		printf("%d %d\n", pjeu->pplateau->largeur, pjeu->pplateau->hauteur);
		Sleep(1000);
	}

	pjeu->pplateau->largeur *= 10;
	pjeu->pplateau->hauteur *= 10;
}

void creerPlateau(jeu *pjeu)
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
			;
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

	int H = pjeu->pplateau->largeur / 10;
	int L = pjeu->pplateau->hauteur / 10;

	pjeu->terres = (int **)mallop(sizeof(int **) * H);
	for (int i = 0; i < H; i++)
	{
		pjeu->terres[i] = (int *)mallop(sizeof(int *) * L);
		for (int j = 0; j < L; j++)
			pjeu->terres[i][j] = 0;
	}

	pjeu->terres[0][0] = 1;
}

void insFinEntite(jeu *pjeu, entite *pb)
{
	if (pjeu->listeEntits->nbrElements == 0)
		pjeu->listeEntits->tete = pb;

	else
	{
		entite *ce = pjeu->listeEntits->tete;
		while (ce->suivant != NULL)
			ce = ce->suivant;

		ce->suivant = pb;
	}
	pjeu->listeEntits->nbrElements++;
}

void insListeE(jeu *pjeu)
{
	pjeu->listeEntits = (listeE *)mallop(sizeof(listeE)); // déclaration
	pjeu->listeEntits->tete = NULL;
	pjeu->listeEntits->nbrElements = 0;
}

void insListeP(jeu *pjeu)
{
	pjeu->plisteP = (listeP *)mallop(sizeof(listeP)); // déclaration
	pjeu->plisteP->tete = NULL;
	pjeu->plisteP->nbrElements = 0;
}

void generer_nonConstr(jeu *pjeu)
{
	srand(clock());
	int elements = pjeu->pplateau->hauteur * pjeu->pplateau->largeur;
	elements *= 0.025;
	for (int i = 0; i < elements; i++)
	{
		entite *cailloux = (entite *)mallop(sizeof(entite));
		cailloux->ID = 0;
		cailloux->hauteur = (rand() % 2) + 1;
		cailloux->largeur = (rand() % 2) + 1;
		cailloux->x = rand() % (pjeu->pplateau->hauteur - cailloux->hauteur - 2) + 1;
		cailloux->y = rand() % (pjeu->pplateau->largeur - cailloux->largeur - 2) + 1;
		cailloux->actif = 1;
		cailloux->suivant = NULL;
		insFinEntite(pjeu, cailloux);
	}
}

void creerTab2d(jeu *pjeu)
{
	pjeu->pplateau->map = (int **)mallop((pjeu->pplateau->largeur) * sizeof(int *));

	for (int i = 0; i < pjeu->pplateau->largeur; i++)
		(pjeu->pplateau->map)[i] = (int *)mallop((pjeu->pplateau->hauteur) * sizeof(int));
}

void dessinerBat(jeu *pjeu, entite *pb, int mode)
{
	for (int k = pb->x; k < pb->x + pb->largeur; k++)
	{
		for (int l = pb->y; l < pb->y + pb->hauteur; l++)
			(pjeu->pplateau->map)[l][k] = mode;
	}
}

char *getEntiteName(int id)
{
	switch (id)
	{
	case 0:
		return "Cailloux";

	case 1:
		return "Route";

	case 2:
		return "Banque";

	case 3:
		return "Maison";

	case 4:
		return "Ferme";

	default:
		printf("ID non valide: %d\nAppuyez pour continuer.\n", id);
		getchar();
		return "[Error]";
	}
}

void getEntiteVals(int id, int tab[])
{
	// --IDs--
	// 0: cailloux
	// 1: route
	// 2: banque
	// 3: maison
	// 4: ferme

	// hauteur, largeur, capacité, prix, RM, RN

	switch (id)
	{
	case 0:
		tab[0] = 0;
		tab[1] = 0;
		tab[2] = 0;
		tab[3] = 0;
		tab[4] = 0;
		tab[5] = 0;
		break;

	case 1:
		tab[0] = 1;
		tab[1] = 1;
		tab[2] = 0;
		tab[3] = 5;
		tab[4] = 0;
		tab[5] = 0;
		break;

	case 2:
		tab[0] = 3;
		tab[1] = 3;
		tab[2] = 0;
		tab[3] = 70;
		tab[4] = 0;
		tab[5] = 0;
		break;

	case 3:
		tab[0] = 2;
		tab[1] = 2;
		tab[2] = 3;
		tab[3] = 35;
		tab[4] = 10;
		tab[5] = 0;
		break;

	case 4:
		tab[0] = 2;
		tab[1] = 3;
		tab[2] = 0;
		tab[3] = 50;
		tab[4] = 0;
		tab[5] = 10;
		break;

	default:
		printf("ID non valide: %d\nLe jeu va crasher.\n", id);
		exit(0);
	}
}

void refreshTab(jeu *pjeu, int last)
{
	for (int i = 0; i < pjeu->pplateau->largeur; i++)
	{
		for (int j = 0; j < pjeu->pplateau->hauteur; j++)
		{
			if (pjeu->terres[i / 10][j / 10])
				(pjeu->pplateau->map)[i][j] = 1;
			else
				(pjeu->pplateau->map)[i][j] = 0;
		}
	}

	entite *pb = pjeu->listeEntits->tete;
	while (pb && (last || pb->suivant))
	{
		int mode = 0;

		if (pb->ID == 0)
			mode = 2;
		else if (pb->ID == 1)
			if (pb->actif == -1)
				mode = 8;
			else
				mode = (pb->actif == 0) ? 3 : 4;
		else if (pb->actif == -1)
			mode = 8;
		else
			mode = 5 + pb->actif;

		dessinerBat(pjeu, pb, mode);
		pb = pb->suivant;
	}
}

void afficherTab(jeu *pjeu)
{
	refreshTab(pjeu, 1);
	printf("\n");

	for (int j = 0; j < pjeu->pplateau->hauteur + 2; j++)
		printf(" \033[34;1m#\033[0m");
	printf("\n");

	for (int i = 0; i < pjeu->pplateau->largeur; i++)
	{
		printf(" \033[34;1m#\033[0m");
		for (int j = 0; j < pjeu->pplateau->hauteur; j++)
		{
			int mode = (pjeu->pplateau->map)[i][j];

			// -- modes --
			// 0: vide vide
			// 1: terres . gris
			// 2: non constructible O gris
			// 3: curseur + vert
			// 4: route + blanc
			// 5: placement # bleus
			// 6: construction # rouge
			// 7: construit # blanc
			// 8: Colision

			if (SHOW_MAP_NUM)
				printf(" %d", mode);
			else
			{
				switch (mode)
				{
				case 0:
					printf("  ");
					break;

				case 1:
					printf(" \033[30;1m.\033[0m");
					break;

				case 2:
					printf(" \033[30;1mO\033[0m");
					break;

				case 3:
					printf(" \033[32;1m+\033[0m");
					break;

				case 4:
					printf(" +");
					break;

				case 5:
					printf(" \033[32;1m#\033[0m");
					break;

				case 6:
					printf(" \033[33m#\033[0m");
					break;

				case 7:
					printf(" #");
					break;

				case 8:
					printf(" \033[31;1m#\033[0m");
					break;
				}
			}
		}
		printf(" \033[34;1m#\033[0m\n");
	}

	for (int j = 0; j < pjeu->pplateau->hauteur + 2; j++)
		printf(" \033[34;1m#\033[0m");
	printf("\n");
}

void insFinPret(jeu *pjeu, pret *pp)
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

void retirerFin(jeu *pjeu)
{
	entite *ce = pjeu->listeEntits->tete;

	if (ce->suivant == NULL)
	{
		free(ce);
		pjeu->listeEntits->tete = NULL;
	}
	else
	{
		while (ce->suivant->suivant)
			ce = ce->suivant;

		free(ce->suivant);
		ce->suivant = NULL;
	}

	pjeu->listeEntits->nbrElements--;
}

entite *allocEntite(jeu *pjeu)
{
	entite *pb = (entite *)mallop(sizeof(entite));
	pb->suivant = NULL;
	pb->actif = 0;
	pb->lvl = 1;
	pb->x = 0;
	pb->y = 0;
	pb->jconst = pjeu->days;

	return pb;
}

void construireBat(jeu *pjeu)
{
	entite *pb = allocEntite(pjeu);
	insFinEntite(pjeu, pb);

	int done = 0, choix = 1;

	while (!done)
	{
		system("cls");
		printf("Type d'entite:\n");

		for (int i = 1; i < 5; i++)
		{
			int vals[6];
			getEntiteVals(i, vals);
			char *name = getEntiteName(i);
			printf(choix == i ? " > " : "   ");
			printf(pjeu->pplayer->money >= vals[3] ? "%s\n" : "\033[30;1m%s\033[0m\n", name);
		}

		printf("\n%sAnnuler\n", choix == 5 ? " > " : "   ");

		while (!kbhit())
			;

		char key = getch();
		if (key == 'H' && choix > 1)
			choix--;

		if (key == 'P' && choix < 5)
			choix++;

		int valsChoix[6];
		if (choix < 5)
			getEntiteVals(choix, valsChoix);

		if (key == '\r' && pjeu->pplayer->money >= valsChoix[3])
			done = 1;
	}

	if (choix == 5)
	{
		retirerFin(pjeu);
		return;
	}

	done = 0;
	while (!done)
	{
		system("cls");

		int vals[6];
		getEntiteVals(choix, vals);
		pb->ID = choix;
		pb->largeur = vals[0];
		pb->hauteur = vals[1];
		pb->capacite = vals[2];
		pb->prix = vals[3];
		pb->revenusM = vals[4];
		pb->revenusN = vals[5];

		if (pjeu->pplayer->money < pb->prix)
			printf("\n\033[31;1m - Plus assez d'argent - \033[0m\n\n");

		refreshTab(pjeu, 0); // 0: On ne voit pas le batiment

		pb->actif = (pjeu->pplayer->money < pb->prix) ? -1 : 0;

		if (pb->actif == 0)
		{
			for (int i = pb->x; i < pb->x + pb->largeur; i++)
			{
				for (int j = pb->y; j < pb->y + pb->hauteur; j++)
				{
					if ((pjeu->pplateau->map)[j][i] != 1)
						pb->actif = -1;
				}
			}
		}

		refreshTab(pjeu, 1); // 1: On voit le batiment
		afficherTab(pjeu);

		printf("\n");
		printf("[B]: Placer\n");
		printf("[T]: Terminer\n");

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

		if (key == 'b' && pb->actif == 0)
		{
			pb->actif = 1;
			pjeu->pplayer->money -= pb->prix;
			int x = pb->x;
			int y = pb->y;
			pb = allocEntite(pjeu);
			insFinEntite(pjeu, pb);
			pb->x = x;
			pb->y = y;
		}

		if (key == 't')
		{
			done = 1;
			retirerFin(pjeu);
		}
	}
}

void newPret(jeu *pjeu)
{
	pjeu->pplayer->money *= 3;
	pret *npret = (pret *)mallop(sizeof(pret));
	npret->debut = pjeu->days;
	npret->somme = 3 * pjeu->pplayer->money;
	npret->timer = 5 * 365;
	npret->suivant = NULL;
	insFinPret(pjeu, npret);
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
	entite *cb = pjeu->listeEntits->tete;
	while (cb)
	{
		if (pjeu->days - cb->jconst > 7 && cb->actif == 1)
		{
			cb->actif = 2;
			pjeu->pplayer->gainsM += cb->revenusM * cb->lvl;
			pjeu->pplayer->gainsN += cb->revenusN * cb->lvl;
		}
		cb = cb->suivant;
	}
}

void creerPlayer(jeu *pjeu)
{
	pjeu->pplayer = (player *)mallop(sizeof(player));
	printf("Nom du joueur: ");
	fgets(pjeu->pplayer->name, 19, stdin);
	pjeu->pplayer->money = 100;
	pjeu->pplayer->gainsM = 0;
	pjeu->pplayer->gainsN = 0;
}

void afficherBat(entite *pb)
{
	printf("%s\n", getEntiteName(pb->ID));

	printf("niveau: %d\n", pb->lvl);
	printf("revenusM: %d\n", pb->revenusM);
	printf("revenusN: %d\n", pb->revenusN);
}

void upgrade(jeu *pjeu)
{
	if (pjeu->listeEntits->nbrElements > 0)
	{

		entite *cb = pjeu->listeEntits->tete;
		char choix;

		while (cb)
		{
			if (cb->ID > 1)
			{
				afficherBat(cb);
				if (pjeu->pplayer->money < cb->prix * cb->lvl)
					printf("\n\033[31;1m - Plus assez d'argent - \033[0m\n\n");
				else
				{

					printf("Voulez vous ameliorer ?(o/n)");
					scanf("%c", &choix);
					viderBuffer();
					if (choix == 'o')
					{
						cb->actif = 1;
						cb->jconst = pjeu->days;
						pjeu->pplayer->gainsM -= cb->revenusM * cb->lvl;
						pjeu->pplayer->gainsN -= cb->revenusN * cb->lvl;
						cb->lvl += 1;
						pjeu->pplayer->money -= cb->prix * cb->lvl;
						cb->revenusM *= 1.25;
						cb->revenusN *= 1.25;
					}
					printf("\n");
				}
			}
			cb = cb->suivant;
		}
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
		pjeu->plisteP->nbrElements--;
	}
}

float random()
{
	return (float)(rand() % 100000) / 100000.0;
}

void ev_Pop(jeu *pjeu)
{

	if (pjeu->nourriture < pjeu->population)
	{
		pjeu->natalite -= 0.5 / 365.0 / 10.0;
		pjeu->mortalite += 0.3 / 365.0 / 10;
	}

	if (pjeu->nourriture > pjeu->population)
	{
		pjeu->natalite += 0.5 / 365.0 / 10.0;
		pjeu->mortalite -= 0.3 / 365.0 / 10;
	}

	for (int i = 0; i < pjeu->population; i++)
	{
		if (random() < pjeu->natalite)
			pjeu->population += 1;

		if (random() < pjeu->mortalite)
			pjeu->population -= 1;
	}
}

void ev_Nouriture(jeu *pjeu)
{
	pjeu->nourriture += pjeu->pplayer->gainsN;
	pjeu->nourriture -= pjeu->population;
	if (pjeu->nourriture <= 0)
		pjeu->nourriture = 0;
}

int state(jeu *pjeu)
{
	if (pjeu->population <= 0 && (pjeu->pplayer->money <= 0 || pjeu->pplayer->gainsM == 0))
	{
		system("cls");
		// print
		return 0;
	}
	return 1;
}

void acheterTerrain(jeu *pjeu)
{
	int c = 0, p = 0;

	for (int i = 0; i < pjeu->pplateau->largeur / 10; i++)
	{
		for (int j = 0; j < pjeu->pplateau->hauteur / 10; j++)
		{
			printf("%d %d %d %d %d %d\n", pjeu->pplateau->largeur, pjeu->pplateau->hauteur, i, j, pjeu->terres[j][i], c);
			c += pjeu->terres[i][j];
		}
		printf("reboot\n");
	}
	printf("sorti\n");

	p = c * pjeu->prixTerre;

	if (pjeu->pplayer->money >= p)
	{
		for (int i = 0; i < pjeu->pplateau->hauteur; i++)
		{
			for (int j = 0; j < pjeu->pplateau->largeur; j++)
			{
				if (pjeu->terres[i][j] == 0)
				{
					pjeu->terres[i][j] = 1;
					pjeu->pplayer->money -= p;
					return;
				}
			}
		}
	}
	printf("fin\n");
}