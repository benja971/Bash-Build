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
	plateau *p = creerPlateau();
	player *j = creerPlayer();
	liste *listeBats = insListe();

	creerTab2d(&p);
	initTab(&p);

	while (continuer)
	{
		day++;
		j->money += j->gains;
		system("cls");
		printf("/ Jour %d / %de / +%d par jours/\n", day, j->money, j->gains);

		afficherTab(p);

		printf("Nouveau batiment: b\n");
		printf("Nouveau pret sur 5 ans: p\n");

		while (!kbhit())
			_sleep(100);

		char key = getch();
		if (key == 'p')
			j->money *= 3;

		if (key == 'b')
			creerBatiment(p, j, listeBats);
	}

	return 0;
}
