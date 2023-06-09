#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utilities.c"

/*
                            << MAIN.C >>
    Fichiers corps du jeu avec les structures ainsi que leurs fonctions associées :

    - init_grille(struct Grille *grille)
    - lier_tuile(struct Grille *grille, struct Tuile *tuile)
    - lier_all_tuiles(struct Grille *grille)
    - afficher_grille(struct Grille *grille)
    - remplir_grille(struct Grille *grille, int *liste_tuiles)
    - mouv_possibles_maj(struct Grille *grille)
    - tuile_swap(struct Grille *grille, int xt, int yt)
    - dupliquer_grille(struct Grille *grille)
    - deplacer(struct Grille *grille,int direction)
    - grille_load(struct Grille *grille,char* fichier)

    - DEBUG : tuile_info(struct Grille *grille, int x, int y)

    _ Tuile
    _ Grille
*/

#define N 4 // Taille du jeu
// Mouvements de la case vide
//                       0UP  1DOWN 2LEFT 3RIGHT
bool mouv_possibles[4] = {true,true,true,true};
#define UP 0 
#define DOWN 1 
#define LEFT 2 
#define RIGHT 3

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
    struct Tuile *grid[N][N];// Grille de jeu de taille N par N
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
            grille->grid[i][j] = malloc(sizeof(struct Tuile)); // Allocation mémoire dynamique de la tuile
            grille->grid[i][j]->num = -1;                       // Numero de la tuile, par défaut 1
            grille->grid[i][j]->x = i;                         // coordonnée x de la tuile
            grille->grid[i][j]->y = j;                         // coordonnée y de la tuile
            grille->grid[i][j]->width = 1;                     // taille simple par défaut
            grille->grid[i][j]->height = 1;                    // ''
        }
    }
    // Initialisation de la case vide
    grille->move_used = -1;
    grille->grid[N - 1][N - 1]->num = 0;
    grille->empty_x = N - 1, grille->empty_y = N - 1;
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
        struct Tuile *tuile = malloc(sizeof(struct Tuile)); // Allocation mémoire dynamique de la tuile.
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
    if (grille->grid[x][y]->part == NULL)
        printf("part coord. (x,y) : AUCUNE \n");
    else
        printf("part coord. (x,y) : (%d,%d) \n", grille->grid[x][y]->part->x,grille->grid[x][y]->part->y);
    printf("empty de la grille : (%d,%d)\n", grille->empty_x, grille->empty_y);
    printf("----------------------------------------------------\n");
}

void mouv_possibles_maj(struct Grille *grille)
// Fonction qui met à jour les mouvements jouables en fonction de la position de la case vide.
{
    for (int i = 0; i < 4; i++)
    {
       mouv_possibles[i] = true;
    }
    
    int x0 = grille->empty_x;
    int y0 = grille->empty_y;

    if (x0 == 0 || grille->grid[x0-1][y0]->width == 2)
        mouv_possibles[0] = false;
    if (x0 == N-1 || grille->grid[x0+1][y0]->width == 2)
        mouv_possibles[1] = false;
    if (y0 == 0 || grille->grid[x0][y0-1]->height == 2)
        mouv_possibles[2] = false;
    if (y0 == N-1 || grille->grid[x0][y0+1]->height == 2)
        mouv_possibles[3] = false;
    
    //debug
    // printf("UP :%s DOWN:%s LEFT:%s RIGHT:%s\n",mouv_possibles[0]?"true":"false",mouv_possibles[1]?"true":"false",mouv_possibles[2]?"true":"false",mouv_possibles[3]?"true":"false");
}

void tuile_swap(struct Grille *grille, int xt, int yt)
{
    int x0 = grille->empty_x, y0 = grille->empty_y;

    struct Tuile *temp = grille->grid[x0][y0]; // Stockage temporaire de la première tuile

    if (grille->grid[xt][yt]->height == 1 && grille->grid[xt][yt]->width == 1)
    {
        grille->grid[x0][y0] = grille->grid[xt][yt]; // La première tuile prend la place de la seconde
        grille->grid[xt][yt] = temp; // La seconde tuile prend la place de la première
        // Mise à jour des coordonnées x et y pour la tuile vide de la grille
        grille->empty_x = xt, grille->empty_y = yt;;
        // Mise à jour des coordonnées x et y pour chaque tuile
        grille->grid[x0][y0]->x = x0, grille->grid[x0][y0]->y = y0;
        grille->grid[xt][yt]->x = xt, grille->grid[xt][yt]->y = yt;
    }
    else if (grille->grid[xt][yt]->height == 2 && grille->grid[xt][yt]->width == 1)
    // si c'est une tuile longue
    {
        grille->grid[x0][y0] = grille->grid[xt-1][yt]; // La première tuile prend la place de la seconde
        grille->grid[xt-1][yt] = temp; // La seconde tuile prend la place de la première
        // Mise à jour des coordonnées x et y pour la tuile vide de la grille
        grille->empty_x = xt-1, grille->empty_y = yt;
        // Mise à jour des coordonnées x et y pour chaque tuile
        grille->grid[x0][y0]->x = x0, grille->grid[x0][y0]->y = y0;
        grille->grid[xt-1][yt]->x = xt-1, grille->grid[xt-1][yt]->y = yt;
    }
    else if (grille->grid[xt][yt]->height == 1 && grille->grid[xt][yt]->width == 2)
    // si c'est une tuile large
    {
        grille->grid[x0][y0] = grille->grid[xt][yt-1]; // La première tuile prend la place de la seconde
        grille->grid[xt][yt-1] = temp; // La seconde tuile prend la place de la première
        // Mise à jour des coordonnées x et y pour la tuile vide de la grille
        grille->empty_x = xt, grille->empty_y = yt-1;
        // Mise à jour des coordonnées x et y pour chaque tuile
        grille->grid[x0][y0]->x = x0, grille->grid[x0][y0]->y = y0;
        grille->grid[xt][yt-1]->x = xt, grille->grid[xt][yt-1]->y = yt-1;
    }
}

struct Grille dupliquer_grille(struct Grille *grille) {
    struct Grille *res = malloc(sizeof(struct Grille)); // Allouer de la mémoire pour la grille dupliquée
    res->h = grille->h, res->g = grille->g;
    res->move_used = grille->move_used;
    res->empty_x = grille->empty_x, res->empty_y = grille->empty_y;

    // Dupliquer chaque tuile de la grille
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Sinon, dupliquer la tuile
            struct Tuile *tuile_d = malloc(sizeof(struct Tuile));
            tuile_d->num = grille->grid[i][j]->num;
            tuile_d->x = grille->grid[i][j]->x;
            tuile_d->y = grille->grid[i][j]->y;
            tuile_d->width = grille->grid[i][j]->width;
            tuile_d->height = grille->grid[i][j]->height;
            tuile_d->isemptyone = grille->grid[i][j]->isemptyone;

            if (tuile_d->width == 2) { // Si la tuile est longue, dupliquer la seconde partie
                struct Tuile *part_dupliquee = malloc(sizeof(struct Tuile));
                part_dupliquee->num = grille->grid[i][j]->part->num;
                part_dupliquee->x = grille->grid[i][j]->part->x;
                part_dupliquee->y = grille->grid[i][j]->part->y;
                part_dupliquee->width = grille->grid[i][j]->part->width;
                part_dupliquee->height = grille->grid[i][j]->part->height;
                part_dupliquee->isemptyone = grille->grid[i][j]->part->isemptyone;

                tuile_d->part = part_dupliquee;
            } else
                tuile_d->part = NULL;

            res->grid[i][j] = tuile_d;
        }
    }
    return *res;
}

void deplacer(struct Grille *grille,int direction)
{
    // vecteurs directionnels avec les coordonnées des cellules voisines (x,y) d'une tuile de la grille
    // coord de la tuile du haut = (x-1,y),du bas = (x+1,y),de gauche = (x,y+1) et de droite = (x,y-1)
    int vect_x[4]  = {-1, 1, 0, 0};
    int vect_y[4] = {0, 0, -1, 1};
    int x0 = grille->empty_x, y0 = grille->empty_y;
    tuile_swap(grille, x0 + vect_x[direction], y0 + vect_y[direction]);
}

int grille_load(struct Grille *grille,char* fichier)
{
    /* Fonction qui prend une grille non init. ainsi que le nom d'un fichier comportant une grille N*N.
       Remplie la grille demandée tel que présenté dans le fichier et renvoie la taille du jeu. */

    int lettre,num,l=1;
    FILE *fd = fopen(fichier, "r");
    FILE *fd2 = fopen(fichier, "r");
    // On parcours toutes les lignes afin d'avoir la taille du jeu dans "l"
    while( (lettre = fgetc(fd)) !=EOF)
    {
        if (lettre == '\n')
        l++;
    }

    int user_liste[l*l];
    // Puis on parcours tout les caractères du fichier et on les ajoutes à notre vecteur "user_liste"
    for (int i = 0; i < l*l; i++)
    {
        fscanf(fd2, "%d", &num);
        // printf("user_liste[%d] = %d\n",i,user_liste[i]); //Debug
        user_liste[i] = num;
    }
    fclose(fd);
    fclose(fd2);
    init_grille(grille);
    grille->move_used = 4;
    remplir_grille(grille,user_liste);
    lier_all_tuiles(grille);

    return l;
}

struct Grille *grilles_voisines(struct Grille *grille){
/*
    Fonction qui retourne une liste de * de grilles dites voisines, en fonctions des coups possibles de la grille.
    On suppose donc que la liste des coups possibles est à jour avec la grille passée en paramètres.
*/
    int nb_v;
    struct Grille temp, *liste_v = malloc(nb_v*sizeof(struct Grille)); //liste de nb_v pointeurs de grille
    mouv_possibles_maj(grille);
    for (int i = 0; i < 4; i++)
    /*
        Si le coup est possible et que ce n'est pas le coup utilisé pour arriver à la grille,
        on incrémente le nombre de coups possibles.
    */
    {
        if(mouv_possibles[i])
        {
            if ((grille->move_used == RIGHT && i == LEFT) || (grille->move_used == DOWN && i == UP))
                mouv_possibles[i] = false;
            if ((grille->move_used == LEFT && i == RIGHT) || (grille->move_used == UP && i == DOWN))
                mouv_possibles[i] = false;
            else
                nb_v++; // Autrement on corrige la liste des coups possibles.
        }
    }

    for (int i = 0; i < nb_v; i++)
    /*
        Pour chaque coups dorénavant possible, on duplique la grille actuelle et on applique le coup,
        puis on ajoute l'adresse de cette nouvelle grille voisine à la liste. On oublie pas de modifier
        "move_used" de la grille avec le coup utilisé.
    */
    {
        if (mouv_possibles[i])
        {
            liste_v[i] = dupliquer_grille(grille);
            deplacer(&liste_v[i],i);
            liste_v[i].move_used = i;
            // printf("\tCoup : %d",liste_v[i].move_used = i); //Debug
            // afficher_grille(&liste_v[i]); //Debug
        }
        else
        {
            init_grille(&liste_v[i]);
            // printf("\tCoup Impossible"); //Debug
            // afficher_grille(&liste_v[i]); //Debug

        }
        
    }

    return liste_v;
}