#include <stdio.h>
#include <stdlib.h>

void * mallop(size_t size) {
    void *p = NULL;
    p = malloc(size);

    if (p == NULL) {
        printf("Erreur de memoire.\n");
        exit(0);
    }

    return p;
}

void viderBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF)
        c = getchar();
}

liste *insListe() {
    liste *liste = mallop(sizeof(liste)); // déclaration

    liste->tete = NULL;
    liste->nbrBats = 0;

    return liste;
}

plateau *creerPlateau() {
    plateau *p = (plateau *)mallop(sizeof(plateau));
    
    p->largeur = 20;
    p->hauteur = 30;

    return p;
}

void creerTab2d(plateau **p) {
    int **tab = (int **)mallop((*p)->largeur * sizeof(int *));

    for (int i = 0; i < (*p)->largeur; i++)
        tab[i] = (int *)mallop((*p)->hauteur * sizeof(int));

    (*p)->map = tab;
}

void initTab(plateau **p) {
    for (int i = 0; i < (*p)->largeur; i++) {
        for (int j = 0; j < (*p)->hauteur; j++)
            ((*p)->map)[i][j] = 0;
    }
}

void afficherTab(plateau *p){
    printf("\n");

    for (int i = 0; i < p->largeur; i++){
        for (int j = 0; j < p->hauteur; j++) {
            int b = (p->map)[i][j];
            if (b)
                printf("%3d", b);
            else
                printf("  .");
        }

        printf("\n");
    }

    printf("\n");
}

void acheterBatiment(batiment *b, player *p) {
    p->money -= b->prix;
}

void ajouterBat(plateau **p, batiment *b) {
    for (int i = 0; i < (*p)->largeur; i++) {
        for (int j = 0; j < (*p)->hauteur; j++) {

            if (b->x == i && b->y == j) {
                for (int k = i; k < i + b->largeur; k++) {
                    for (int l = j; l < j + b->hauteur; l++)
                        ((*p)->map)[l][k] = 1;
                }
            }
        }
    }
}

void InsFin(liste *liste, plateau *p, player *j, batiment *cb) {
    if (liste->nbrBats == 0)
        liste->tete = cb;

    else {
        batiment *bb = liste->tete;
        while (bb->suivant != NULL) // parcours de la liste
            bb = bb->suivant;

        bb->suivant = bb;
    }

    liste->nbrBats += 1;
}

batiment *creerBatiment(plateau *p, player *j, liste *liste) {
    batiment *b = (batiment *)mallop(sizeof(batiment));

    b->largeur = 3;
    b->hauteur = 2;

    viderBuffer();
    printf("Type de batiment: ");
    fgets(b->name, 19, stdin);

    printf("Position x du batiment: ");
    scanf("%d", &b->x);

    printf("Position y du batiment: ");
    scanf("%d", &b->y);

    b->prix = 70;
    b->suivant = NULL;

    if (b->x + b->largeur > 10 || b->y + b->hauteur > 10)
        printf("\nImpossible de construire ce batiment ici.\n");

    else if (b->prix > j->money)
        printf("\nVous n'avez pas suffisement d'argent pour ca.\n");

    else if (b->prix < j->money) {
        acheterBatiment(b, j);
        InsFin(liste, p, j, b);
        ajouterBat(&p, b);
    }

    return b;
}

player *creerPlayer() {
    player *j = (player *)mallop(sizeof(player));
    printf("Quel est votre nom: ");
    fgets(j->name, 19, stdin);
    j->money = 100;
}