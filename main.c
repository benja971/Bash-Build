#include <stdio.h>
#include <stdlib.h>
#include "Structs.c"
#include "Fonctions.c"
#include <stdbool.h>
#include <conio.h>
#include <time.h>

int main() {
    bool continuer = true;
    int choix;
    plateau *p = creerPlateau();
    player *j = creerPlayer();
    liste *listeBats = insListe();
    int day = 0;

    creerTab2d(&p);
    initTab(&p);
    
    while (continuer) {
        day++;

        system("cls");
        printf("/ Jour %d / %de /\n", day, j->money);

        afficherTab(p);

        printf("Nouveau batiment: b\n");
        printf("Nouveau pret sur 5 ans: p\n");

        while (!kbhit()) _sleep(100);

        char key = getch();
        if (key == 'p')
            j->money *= 3;

        // switch (choix) {
        // case 0:
        //     printf("Le programme s'est deroule sans problemes.");
        //     exit(0);

        // case 1:
        //     creerBatiment(p, j, listeBats);
        //     break;

        // case 2:

        //     break;
        // }

        // viderBuffer();
    }

    return 0;
}
