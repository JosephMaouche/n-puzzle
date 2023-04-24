#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "main.c"

/*
                            << ALGO.C >>
    Fichiers annexe du jeu avec des fonctions de calculs et recherches relaté à la résolution :

    - heuristic(struct Grille *i, struct Grille *g)
    - compter_inversions(struct Grille *grille)
    - solvable(struct Grille *grille)
    
*/

int heuristic(struct Grille *i, struct Grille *g)
// Calcul l'heuristique de la grille (i) en fonction de sa grille de fin (g)
{
    int h = 0;
    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < N; y++)
        {
            // Dès qu'une tuile n'est pas à sa place et que ce n'est pas la case vide, on incrémente le score de 1.
            if (i->grid[x][y]->num != g->grid[x][y]->num && i->grid[x][y]->num > 0)
                h++; 
        }
    }
    // On met à jour la valeur heuristique de la grille.
    i->h = h;
    return h;
}

// Compte le nombre d'inversions dans la grille
int compter_inversions(struct Grille *grille) {
    int inversions = 0;
    for (int i = 0; i < N * N - 1; i++) {
        int i_x = i / N; // Coordonnée x de l'élément i
        int i_y = i % N; // Coordonnée y de l'élément i
    
    if (grille->grid[i_x][i_y]->num == 0) {
        continue; // Ignorer la case vide
    }
    
    // Si la tuile est de taille 2, il y a une inversion supplémentaire
    if (grille->grid[i_x][i_y]->width == 2) {
        if (i_y + 1 >= N) {
            continue; // Ignorer si la tuile de taille 2 est à la fin de la ligne
        }
        if (grille->grid[i_x][i_y]->num > grille->grid[i_x][i_y+1]->num) {
            inversions++;
        }
    }

    if (grille->grid[i_x][i_y]->height == 2) {
        if (i_y + 1 >= N) {
            continue; // Ignorer si la tuile de taille 2 est à la fin de la ligne
        }
        if (grille->grid[i_x][i_y]->num > grille->grid[i_x][i_y+1]->num) {
            inversions++;
        }
    }
    
    for (int j = i + 1; j < N * N; j++) {
        int j_x = j / N; // Coordonnée x de l'élément j
        int j_y = j % N; // Coordonnée y de l'élément j
        
        if (grille->grid[j_x][j_y]->num == 0) {
            continue; // Ignorer la case vide
        }
        
        if (grille->grid[i_x][i_y]->num > grille->grid[j_x][j_y]->num) {
            inversions++;
            }
        }
    }
    return inversions;
}

// Vérifie si la grille est résoluble ou non
bool solvable(struct Grille *grille) {
    int inversions = compter_inversions(grille);
// Si N est impair, la grille est résoluble si le nombre d'inversions est pair
    if (N % 2 == 1) {
        return (inversions % 2 == 0);
    }

// Si N est pair, la grille est résoluble si le nombre d'inversions est pair et que la case vide est sur une rangée impaire à partir du bas ou si le nombre d'inversions est impair et que la case vide est sur une rangée paire à partir du bas.
    else {
        int empty_x, empty_y;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (grille->grid[i][j]->num == 0) {
                    empty_x = i;
                    empty_y = j;
                }
            }
        }
        if (empty_x % 2 == 1) {
            return (inversions % 2 == 0);
        }
        else {
            return (inversions % 2 == 1);
        }
    }
}

int main()
{
    struct Grille grille_de_test;
    char finit[] = "init.txt";
    struct Grille grille_goal;
    char fgoal[] = "goal.txt";
    grille_load(&grille_de_test, finit);
    grille_load(&grille_goal, fgoal);
    // heuristic(&grille_de_test, &grille_goal,3);
    // tuile_info(&grille_de_test,2,2);
    int h = heuristic(&grille_de_test, &grille_goal);
    printf("H score ? : %d",h);
    // afficher_grille(&grille_goal);
    // afficher_grille(&grille_de_test);
}