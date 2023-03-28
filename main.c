#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 4 // Taille du jeu

// Structure d'une tuile :
struct Tuile
{
    int num;            // Numéro de la tuile
    int x;              // coordonnée x de la tuile
    int y;              // coordonnée y de la tuile
    int width;          // largeur de la tuile (1 pour une tuile simple, 2 pour une tuile longue)
    int height;         // hauteur de la tuile (1 pour une tuile simple, 2 pour une tuile longue)
    struct Tuile *part; // seconde partie de la tuile si elle est longue, autrement c'est NULL.
};

// Structure d'une grille :
struct Grille
{
    struct Tuile *grid[N][N];   //Grille de jeu de taille N par N
    int h;                      //Valeur heuristique à utiliser plus tard ?
};

// Initialisation de la grille avec (N*N - 1) Tuiles :
void init_grille(struct Grille *grille)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            grille->grid[i][j] = malloc(sizeof(struct Tuile)); //Allocation memoire dynamique de la tuile
            grille->grid[i][j]->num = 1; // Numero de la tuile, par défaut 1 
            grille->grid[i][j]->x = i;   // coordonnée x de la tuile
            grille->grid[i][j]->y = j;   // coordonnée y de la tuile
            grille->grid[i][j]->width = 1; // taille simple par défaut
            grille->grid[i][j]->height = 1;
        }
    }
    // Initialisation de la case vide
    grille->grid[N - 1][N - 1]->num = -1;
}

void lier_tuile(struct Grille *grille, struct Tuile *tuile)
//Fonction prennant en entrée un pointeur vers une tuile et comapare
//son numéro avec celui de toutes les autres tuiles dans la grille. 
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (grille->grid[i][j]->num == tuile->num && grille->grid[i][j] != tuile)
            {
                //Si elle trouve une autre tuile avec le même numéro, 
                //elle lie les deux tuiles en mettant à jour leur champ "part". 
                //La fonction s'arrête dès qu'elle a trouvé une tuile correspondante.
                printf("La tuile %d est longue\n", grille->grid[i][j]->num); //Debug
                tuile->part = grille->grid[i][j];
                //Si les deux tuiles sont sur le même axe X,il s'agit d'une tuile large et on change alors sa largeur.
                if (tuile->x == grille->grid[i][j]->x)
                {
                    tuile->width = 2;
                }
                //Sinon on change sa hauteur.
                else
                {
                    tuile->height = 2;
                }
                grille->grid[i][j]->part = tuile;
                return;
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
                // Affiche un vide pour la tuile 0, correspondant à la case vide.
                printf("  ");
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
        //Allocation dynamique de la tuile
        struct Tuile *tuile = malloc(sizeof(struct Tuile));
        tuile->num = liste_tuiles[i]; // Assignation du numéro de la tuile
        tuile->x = x; // Assignation de la coordonnée x
        tuile->y = y; // Assignation de la coordonnée x
        tuile->width = 1; // Assignation de la taille, simple par défaut
        tuile->height = 1; // Assignation de la taille, simple par défaut
        tuile->part = NULL; // Assignation de son extension, nul par défaut
        grille->grid[x][y] = tuile; // Ajout de la tuile à la grille
        i++;
    }
}

int main()
{
    struct Grille grille_de_test;
    init_grille(&grille_de_test);
    int liste_tuiles[16] = {1, 2, 3, 3, 4, 4, 5, 6, 7, 8, 5, 9, 10, 11, 12, 0};
    remplir_grille(&grille_de_test, liste_tuiles);
    lier_all_tuiles(&grille_de_test);
    afficher_grille(&grille_de_test);
    return 0;
}