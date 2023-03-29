#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 4 // Taille du jeu
// Mouvements de la case vide
//                          UP  DOWN LEFT RIGHT
bool coups_possibles[4] = {true,true,true,true};

// Structure d'une tuile :
struct Tuile
{
    int num;            // Numéro de la tuile
    int x;              // coordonnée x de la tuile
    int y;              // coordonnée y de la tuile
    int width;          // largeur de la tuile (1 pour une tuile simple, 2 pour une tuile longue)
    int height;         // hauteur de la tuile (1 pour une tuile simple, 2 pour une tuile longue)
    struct Tuile *part; // seconde partie de la tuile si elle est longue, autrement c'est NULL.
    bool isemptyone;    //Si il s'agit de la tuile vide ou non
};

// Structure d'une grille :
struct Grille
{
    struct Tuile *grid[N][N]; // Grille de jeu de taille N par N
    int h;                    // Valeur heuristique (Manhattan Distance)    | Algorithme A*
    int g;                    // Cout                                       | Algorithme A*
    int move_used;            // Mouvement utilisé pour arriver à cette grille 
    int empty_x;              // coordonnée x de la tuile vide
    int empty_y;              // coordonnée y de la tuile vide
};

// Initialisation de la grille avec (N*N - 1) Tuiles :
void init_grille(struct Grille *grille)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            grille->grid[i][j] = malloc(sizeof(struct Tuile)); // Allocation memoire dynamique de la tuile
            grille->grid[i][j]->num = 1;                       // Numero de la tuile, par défaut 1
            grille->grid[i][j]->x = i;                         // coordonnée x de la tuile
            grille->grid[i][j]->y = j;                         // coordonnée y de la tuile
            grille->grid[i][j]->width = 1;                     // taille simple par défaut
            grille->grid[i][j]->height = 1;                    // ''
        }
    }
    // Initialisation de la case vide
    grille->grid[N - 1][N - 1]->num = -1;
    grille->g = 0;  // Valeur par défaut pour le cout 
}

void lier_tuile(struct Grille *grille, struct Tuile *tuile)
// Fonction prennant en entrée un pointeur vers une tuile et comapare
// son numéro avec celui de toutes les autres tuiles dans la grille.
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (grille->grid[i][j]->num == tuile->num && grille->grid[i][j] != tuile)
            {
                // Si elle trouve une autre tuile avec le même numéro,
                // elle lie les deux tuiles en mettant à jour leur champ "part".
                // La fonction s'arrête dès qu'elle a trouvé une tuile correspondante.
                // printf("La tuile %d est longue\n", grille->grid[i][j]->num); // Debug
                tuile->part = grille->grid[i][j];
                // Si les deux tuiles sont sur le même axe X, il s'agit d'une tuile large et on change alors sa largeur.
                if (tuile->x == grille->grid[i][j]->x)
                {
                    tuile->width = 2;
                }
                // Sinon on change sa hauteur.
                else
                {
                    tuile->height = 2;
                }
                grille->grid[i][j]->part = tuile;
            }
            else
            {
                tuile->part = NULL;
            }
        }
    }
}

void lier_all_tuiles(struct Grille *grille)
// Fonction qui appelle de la fonction 'lier_tuile' pour toutes les tuiles
// de la grille afin de lier toutes les tuiles longues trouvées.
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            lier_tuile(grille, grille->grid[i][j]);
        }
    }
}

void afficher_grille(struct Grille *grille)
// Fonction permettant d'afficher esthétiquement la grille.
{
    printf("\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (grille->grid[i][j]->num == 0)
            {
                // Affiche un vide pour la tuile 0, correspondante à la case vide.
                printf("  \t");
            }
            else
            {
                // Affiche le numéro de la tuile suivi d'une tabulation.
                printf("%d\t", grille->grid[i][j]->num);
            }
        }
        printf("\n");
        printf("\n");
    }
}

void remplir_grille(struct Grille *grille, int *liste_tuiles)
// Fonction remplissant une grille à partir d'une liste de numéro.
// La liste doit comporter N*N numéros, en comprenant la case vide (0)
// ainsi que les tuiles longues (répétitions).
{
    int i = 0;
    while (i < N * N)
    {
        int x = i / N;
        int y = i % N;
        struct Tuile *tuile = malloc(sizeof(struct Tuile)); // Allocation dynamique de la tuile.
        tuile->num = liste_tuiles[i];                       // Assignation du numéro de la tuile.
        if (tuile->num == 0)
        {
            grille->empty_x = x;                            //Assignations des coordonnées de la tuile vide
            grille->empty_y = y;
        }
        tuile->x = x;                                       // Assignation de la coordonnée x
        tuile->y = y;                                       // Assignation de la coordonnée x
        tuile->width = 1;                                   // Assignation de la taille, simple par défaut.
        tuile->height = 1;                                  // Assignation de la taille, simple par défaut.
        tuile->part = 0;                                 // Assignation de son extension, nul par défaut.
        tuile->isemptyone = true;
        grille->grid[x][y] = tuile;                         // Ajout de la tuile à la grille.
        i++;
    }
}

void tuile_info(struct Grille *grille, int x, int y)
// Fonction de debug pour voir les infos d'une tuile
{
    printf("----------------------------------------------------\n");
    printf("Info de la tuile (%d,%d) : \n",x,y);
    printf("memAdress : %p\n", &grille->grid[x][y]);
    printf("num : %d\n", grille->grid[x][y]->num);
    printf("width : %d\n", grille->grid[x][y]->width);
    printf("height : %d\n", grille->grid[x][y]->height);
    printf("x,y : %d,%d\n", grille->grid[x][y]->x,grille->grid[x][y]->y);
    printf("part memAdress : %p\n", &grille->grid[x][y]->part);
    printf("empty de la grille : (%d,%d)\n", grille->empty_x, grille->empty_y);
    printf("----------------------------------------------------\n");
}

void coups_possibles_maj(struct Grille *grille)
// Fonction qui met à jour les coups jouables en fonction de la position de la case vide.
{
    int x0 = grille->empty_x;
    int y0 = grille->empty_y;

    if (x0 == 0)
        coups_possibles[0] = false;
    if (x0 == N-1)
        coups_possibles[1] = false;
    if (y0 == 0)
        coups_possibles[2] = false;
    if (y0 == N-1)
        coups_possibles[3] = false;
    
    //debug
    printf("UP:%s\nDOWN:%s\nLEFT:%s\nRIGHT:%s\n",coups_possibles[0]?"true":"false",coups_possibles[1]?"true":"false",coups_possibles[2]?"true":"false",coups_possibles[3]?"true":"false");
}


void tuile_swap(struct Grille *grille, int x0, int y0, int xt, int yt)
{
    
    struct Tuile *temp = grille->grid[x0][y0]; // Stockage temporaire de la première tuile

    if (grille->grid[xt][yt]->height == 1 && grille->grid[xt][yt]->width == 1)
    {
        grille->grid[x0][y0] = grille->grid[xt][yt]; // La première tuile prend la place de la seconde
        grille->grid[xt][yt] = temp; // La seconde tuile prend la place de la première
        // Mise à jour des coordonnées x et y pour la tuile vide de la grille
        grille->empty_x = xt;
        grille->empty_y = yt;
        // Mise à jour des coordonnées x et y pour chaque tuile
        grille->grid[x0][y0]->x = x0;
        grille->grid[x0][y0]->y = y0;
        grille->grid[xt][yt]->x = xt;
        grille->grid[xt][yt]->y = yt;
    }
    else if (grille->grid[xt][yt]->height == 2 && grille->grid[xt][yt]->width == 1)
    // si c'est une tuile longue
    {
        grille->grid[x0][y0] = grille->grid[xt-1][yt]; // La première tuile prend la place de la seconde
        grille->grid[xt-1][yt] = temp; // La seconde tuile prend la place de la première
        // Mise à jour des coordonnées x et y pour la tuile vide de la grille
        grille->empty_x = xt-1;
        grille->empty_y = yt;
        // Mise à jour des coordonnées x et y pour chaque tuile
        grille->grid[x0][y0]->x = x0;
        grille->grid[x0][y0]->y = y0;
        grille->grid[xt-1][yt]->x = xt-1;
        grille->grid[xt-1][yt]->y = yt;
    }
    else if (grille->grid[xt][yt]->height == 1 && grille->grid[xt][yt]->width == 2)
    // si c'est une tuile large
    {
        grille->grid[x0][y0] = grille->grid[xt][yt-1]; // La première tuile prend la place de la seconde
        grille->grid[xt][yt-1] = temp; // La seconde tuile prend la place de la première
        // Mise à jour des coordonnées x et y pour la tuile vide de la grille
        grille->empty_x = xt;
        grille->empty_y = yt-1;
        // Mise à jour des coordonnées x et y pour chaque tuile
        grille->grid[x0][y0]->x = x0;
        grille->grid[x0][y0]->y = y0;
        grille->grid[xt][yt-1]->x = xt;
        grille->grid[xt][yt-1]->y = yt-1;
    }
}

int main()
{
    struct Grille grille_de_test;
    int liste_tuiles_4x4[16] = {1, 2, 3, 3, 4, 4, 5, 6, 7, 8, 5, 9, 10, 11, 12, 0};
    int liste_tuiles_5x5[25] = {1, 2, 3, 3, 4, 4, 5, 6, 7, 8, 5, 9, 10, 11, 12, 2, 3, 2, 3, 2, 3, 2, 3, 2, 0};
    int liste_tuiles_10x10[100] = {87, 23, 56, 91, 12, 78, 45, 67, 34, 89, 76, 98, 54, 21, 43, 65, 90, 32, 10, 55, 88, 11, 44, 77, 99, 22, 57, 33, 66, 79, 13, 46, 68, 35, 80, 24, 58, 92, 14, 47, 70, 36, 81, 25, 59, 93, 15, 48, 71, 38, 83, 26, 60, 94, 16, 49, 72, 39, 84, 27, 61, 95, 17, 50, 73, 40, 85, 28, 62, 96, 18, 51, 74, 41, 86, 29, 63, 97, 19, 52, 75, 42, 64, 82, 20, 53, 69, 37, 31, 30, 18, 51, 74, 41, 86, 29, 63, 97, 19, 0};
    init_grille(&grille_de_test);
    remplir_grille(&grille_de_test, liste_tuiles_4x4);
    lier_all_tuiles(&grille_de_test);
    afficher_grille(&grille_de_test);
    tuile_swap(&grille_de_test, 3,3,3,2);
    afficher_grille(&grille_de_test);
    tuile_swap(&grille_de_test,3,2,2,2);
    afficher_grille(&grille_de_test);
    tuile_swap(&grille_de_test,1,2,1,1);
    afficher_grille(&grille_de_test);
    tuile_info(&grille_de_test,2,0);
    coups_possibles_maj(&grille_de_test);
}