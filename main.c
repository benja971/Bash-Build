#include <stdio.h>
#include <stdlib.h>
#include "Structs.c"
#include "Fonctions.c"
#include <conio.h>
#include <time.h>

int main()
{
	jeu *pjeu = (jeu *)mallop(sizeof(jeu));
	system("cls");
	int continuer = 1;
	pjeu->days = 0;
	pjeu->nourriture = 0;
	pjeu->population = 10;
	pjeu->loges = 0;
	pjeu->natalite = 1 / 365;
	pjeu->mortalite = 1 / (100 * 365);
	pjeu->dy = 0;
	plateau *pp = creerPlateau(pjeu);
	printf("\n");
	player *pj = creerPlayer(pjeu);
	listeB *listeBats = insListeB(pjeu);
	listeP *listePrets = insListeP(pjeu);

	creerTab2d(pjeu);

	while (continuer)
	{
		pjeu->days++;
		pjeu->pplayer->money += pjeu->pplayer->gains;

		system("cls");
		printf("/ Jour %d / %de / +%de /\n", pjeu->days, pjeu->pplayer->money, pjeu->pplayer->gains);
		printf("/ Nourriture %d / %d habitants dont %d loges/\n", pjeu->nourriture, pjeu->population, pjeu->loges);
		printf("/ Pret [%d]:", listePrets->nbrElements);
		afficherPrets(pjeu);
		printf("\n");

		afficherTab(pjeu);
		timerPrets(pjeu);
		evPpop(pjeu);
		printf("Batiments [%d]: ", listeBats->nbrElements);
		batiment *pb = listeBats->tete;
		while (pb != NULL)
		{
			if (pb->ID == 0)
				printf("Banque ");

			if (pb->ID == 1)
				printf("Maison ");

			pb = pb->suivant;
		}
		printf("\n\n");

		printf("[N] Construire batiment\n");
		printf("[B] Nouveau pret sur 5 ans\n");
		printf("[U] Ameliorer un batiment\n");
		printf("[R] Rembourser le premier pret\n");
		printf("[P] Passer un jour\n");
		printf("[Q] Quitter\n");
		printf("\n");

		while (!kbhit())
			;

		majBat(pjeu);

		char key = getch();
		if (key == 'n')
			construireBat(pjeu);

		if (key == 'b')
		{
			pj->money *= 3;
			newPret(pjeu);
		}

		if (key == 'u')
			upgrade(pjeu);

		if (key == 'q')
			continuer = 0;

		if (key == 'r')
			rembourserPret(pjeu);
	}

	return 0;
}
