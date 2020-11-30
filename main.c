#include <stdio.h>
#include <stdlib.h>
#include "Structs.h"
#include "Fonctions.h"
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
	pjeu->natalite = 0.5 / 365.0;
	pjeu->mortalite = 0.3 / 365.0;
	pjeu->dy = 0;
	pjeu->prixTerre = 500;
	creerPlateau(pjeu);
	printf("\n");
	creerPlayer(pjeu);
	insListeE(pjeu);
	insListeP(pjeu);
	creerTab2d(pjeu);
	generer_nonConstr(pjeu);

	while (continuer)
	{

		while (!kbhit())
		{
			pjeu->days++;
			pjeu->pplayer->money += pjeu->pplayer->gainsM;

			system("cls");
			printf("/ Jour %d / %de / +%de /\n", pjeu->days, pjeu->pplayer->money, pjeu->pplayer->gainsM);
			printf("/ %d habitants dont %d loges/ %f / %f /\n", pjeu->population, pjeu->loges, pjeu->natalite, pjeu->mortalite);
			printf("/ Nourriture %d / +%dn /\n", pjeu->nourriture, pjeu->pplayer->gainsN);
			printf("/ Prets [%d]", pjeu->plisteP->nbrElements);
			afficherPrets(pjeu);
			printf(" /\n");

			afficherTab(pjeu);
			majBat(pjeu);
			timerPrets(pjeu);
			ev_Pop(pjeu);
			ev_Nouriture(pjeu);
			printf("Entites [%d]: ", pjeu->listeEntits->nbrElements);
			entite *pb = pjeu->listeEntits->tete;
			while (pb != NULL)
			{
				if (pb->ID != 0 && pb->ID != 1)
					printf("%s ", getEntiteName(pb->ID));
				pb = pb->suivant;
			}
			printf("\n\n");

			printf("[N] Construire\n");
			printf("[B] Nouveau pret sur 5 ans\n");
			printf("[U] Ameliorer un batiment\n");
			printf("[R] Rembourser le premier pret\n");
			printf("[P] Passer un jour\n");
			printf("[E] Nouvelle parcelle\n");
			printf("[Q] Quitter\n");
			printf("\n");
			Sleep(250);
		}
		char key = getch();
		if (key == 'n')
			construireBat(pjeu);

		if (key == 'b')
			newPret(pjeu);

		if (key == 'u')
			upgrade(pjeu);

		if (key == 'q')
			continuer = 0;

		if (key == 'r')
			rembourserPret(pjeu);

		if (key == 'e')
			acheterTerrain(pjeu);
	}
	continuer = state(pjeu);
	return 0;
}
