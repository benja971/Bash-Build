#include <stdio.h>
#include <stdlib.h>
#include "Structs.c"
#include "Fonctions.c"
#include <stdbool.h>

int main()
{
    bool continuer = true;
    int choix;
    plateau *p = creerPlateau();
    player *j = creerPlayer();
    liste *listeBats = InsListe();
    creerTab2d(&p);
    initTab(&p);
    while (continuer)
    {
        // system("cls");
        printf("Que voulez vous faire (1: creer un batiment, 2: faire un pret, 0; sauver et quitter):");
        scanf("%d", &choix);
        printf("\n");
        afficherPerso(j);
        switch (choix)
        {
        case 0:
            printf("Le programme c'est deroule sans problemes.");
            exit(0);

        case 1:
            creerBatiment(p, j, listeBats);
            break;

        case 2:

            break;
        }
        afficherTab(p);
        viderBuffer();
    }

    return 0;
}
