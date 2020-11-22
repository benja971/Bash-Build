#include <stdio.h>
#include <stdlib.h>
#include "Structs.c"
#include "Fonctions.c"
#include "Menu.c"
#include <conio.h>
#include <time.h>

int main()
{
	system("cls");
	int continuer = 1, day = 0;
	plateau *pp = creerPlateau();
	printf("\n");
	player *pj = creerPlayer();
	liste *listeBats = insListe();

	creerTab2d(&pp);

	while (continuer)
	{
		day++;
		pj->money += pj->gains;

		system("cls");
		printf("/ Jour %d / %de / +%de par jour /\n", day, pj->money, pj->gains);
		printf("/ Prets /");
		printf("\n");

		refreshTab(pp, listeBats);
		afficherTab(pp);

		printf("Batiments [%d]: ", listeBats->nbrBats);
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
		printf("[P] Passer un jour\n");
		printf("[Q] Quitter\n");
		printf("\n");

		while (!kbhit())
			;

		majBat(listeBats);

		char key = getch();
		if (key == 'n')
			construireBat(pp, pj, listeBats);

		if (key == 'b')
			pj->money *= 3;

		if (key == 'q')
			continuer = 0;
	}

	return 0;
}
