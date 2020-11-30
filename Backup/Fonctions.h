#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
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

void insFinBats(jeu *pjeu, entite *pb)
{
	if (pjeu->listeEntits->nbrElements == 0)
		pjeu->listeEntits->tete = pb;

	else
	{
		entite *tete = pjeu->listeEntits->tete;
		while (tete->suivant != NULL)
			tete = tete->suivant;

		tete->suivant = pb;
	}

	pjeu->listeEntits->nbrElements++;
}

listeE *insListeE(jeu *pjeu)
{
	pjeu->listeEntits = (listeE *)mallop(sizeof(listeE)); // déclaration
	pjeu->listeEntits->tete = NULL;
	pjeu->listeEntits->nbrElements = 0;
	return pjeu->listeEntits;
}

listeP *insListeP(jeu *pjeu)
{
	pjeu->plisteP = (listeP *)mallop(sizeof(listeP)); // déclaration

	pjeu->plisteP->tete = NULL;
	pjeu->plisteP->nbrElements = 0;

	return pjeu->plisteP;
}

void generer_nonConstr(jeu *pjeu)
{
	int elements = pjeu->pplateau->hauteur * pjeu->pplateau->largeur;
	elements *= 0.1;
	for (int i = 0; i < elements; i++)
	{
		entite *cailloux = (entite *)mallop(sizeof(entite));
		cailloux->ID = 2;
		cailloux->hauteur = rand() % 2 + 1;
		cailloux->largeur = rand() % 2 + 1;
		cailloux->x = rand() % pjeu->pplateau->largeur - cailloux->largeur + 1;
		cailloux->y = rand() % pjeu->pplateau->hauteur - cailloux->hauteur + 1;
		cailloux->capacite = 0;
		cailloux->revenusM = 0;
		cailloux->revenusN = 0;
		cailloux->prix = 0;
		cailloux->actif = 1;
		cailloux->lvl = 0;
		cailloux->jconst = 0;
		insFinBats(pjeu, cailloux);
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
	while (pb != NULL && (!last && pb->suivant == NULL))
	{
		int mode = 0;

		if (pb->ID == 0)
			mode = 2;
		else if (pb->ID == 1)
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
	for (int i = 0; i < pjeu->pplateau->largeur; i++)
	{
		for (int j = 0; j < pjeu->pplateau->hauteur; j++)
		{
			int mode = (pjeu->pplateau->map)[i][j];

			// -- modes --
			// 0: vide . gris
			// 1: terres . blancs
			// 2: non constructible # gris
			// 3: curseur + rouge
			// 4: route + blanc
			// 5: placement # bleus
			// 6: construction # rouge
			// 7: construit # blanc

			switch (mode)
			{
			case 0:
				printf(" \033[30;1m.\033[0m");
				break;

			case 1:
				printf(" .");
				break;

			case 2:
				printf(" \033[30;1m#\033[0m");
				break;

			case 3:
				printf(" \033[34;1m+\033[0m");
				break;

			case 4:
				printf(" +");
				break;

			case 5:
				printf(" \033[34;1m#\033[0m");
				break;

			case 6:
				printf(" \033[33m#\033[0m");
				break;

			case 7:
				printf(" #");
				break;

			case 8:
				printf(" \033[31m#\033[0m");
				break;
			}
		}
		printf("\n");
	}
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

entite *construireBat(jeu *pjeu)
{
	entite *pb = (entite *)mallop(sizeof(entite));
	pb->suivant = NULL;
	pb->actif = 0;
	pb->lvl = 1;
	pb->x = 0;
	pb->y = 0;
	pb->jconst = pjeu->days;

	insFinBats(pjeu, pb);
	int done = 0, choix = 1;

	while (!done)
	{
		system("cls");
		printf("Type de entite:\n");

		for (int i = 1; i < 5; i++)
		{
			int vals[6];
			getEntiteVals(i, vals);
			char *name = getEntiteName(i);
			printf(choix == i ? " > " : "   ");
			printf(pjeu->pplayer->money >= vals[3] ? "%s\n" : "\033[30;1m%s[0m\n", name);
		}
		printf("%s%s\n", choix == 5 ? "\n > " : "   ", "Annuler");

		while (!kbhit())
			;

		char key = getch();
		if (key == 'H' && choix > 0)
			choix--;

		if (key == 'P' && choix < 3)
			choix++;

		int valsChoix[6];
		getEntiteVals(choix, valsChoix);
		if (key == '\r' && pjeu->pplayer->money >= valsChoix[3])
			done = 1;
	}

	if (choix == 4)
		return NULL;
	else
	{
		int vals[6];
		getEntiteVals(choix, vals);
		pb->ID = choix;
		pb->largeur = vals[0];
		pb->hauteur = vals[1];
		pb->capacite = vals[2];
		pb->prix = vals[3];
		pb->revenusM = vals[4];
		pb->revenusN = vals[5];
	}

	done = 0;
	while (!done)
	{
		system("cls");
		printf("position:\n");
		printf("%d, %d\n", pb->x, pb->y);
		refreshTab(pjeu, 0); //0: On ne voit pas le batiment

		for (int i = pb->x; i < pb->x + pb->largeur; i++)
		{
			for (int j = pb->y; j < pb->y + pb->hauteur; j++)
			{
				if ((pjeu->pplateau->map)[i][j] == 1)
					pb->actif = 0;
				else
					pb->actif = -1;
			}
		}
		refreshTab(pjeu, 1); //1: On voit le batiment
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
			done = 1;
	}

	pb->actif = 1;
	pjeu->pplayer->gainsM += pb->revenusM;
	pjeu->pplayer->gainsN += pb->revenusN;
	pjeu->pplayer->money -= pb->prix;

	return pb;
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
		if (pjeu->days - cb->jconst > 2 && cb->actif == 1)
			cb->actif = 2;
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
			afficherBat(cb);
			printf("Voulez vous améliorer ?(o/n)");
			scanf("%c", &choix);
			viderBuffer();
			if (choix == 'o')
			{
				cb->actif = 0;
				cb->jconst = pjeu->days;
				pjeu->pplayer->gainsM -= cb->revenusM;
				cb->revenusM *= 1.1;
				cb->revenusN *= 1.1;
			}
			printf("\n");
			cb = cb->suivant;
		}
	}
	else
	{
		printf("Pas de entites a ameliorer.");
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

	// if (pjeu->nourriture < pjeu->population)
	// {
	// 	pjeu->natalite -= 0.5 / 365.0 / 10.0;
	// 	pjeu->mortalite -= 0.3 / 365.0 / 10;
	// }

	// if (pjeu->nourriture > pjeu->population)
	// {
	// 	pjeu->natalite += 0.5 / 365.0 / 10.0;
	// 	pjeu->mortalite += 0.3 / 365.0 / 10;
	// }

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