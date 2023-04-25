#include "algo.c"

int main()
{
    printf("\n\n\n");
    
    printf("\t\t...JEU DU TAQUIN...\n");
    printf("L1-Informatique PARIS 8 | Joseph MAOUCHE & John RAHARISON\n");
    printf("Taille de la grille de jeu : %d\n",N);
    printf("Chargements des grilles de jeu, assurez-vous bien que les grilles des fichiers\ninit.txt et goal.txt soit corrects...\n");

    // AJOUT DE LA GRILLE DE DÉPART
    char finit[] = "init.txt";
    struct Grille grille_init;
    grille_load(&grille_init, finit);
    printf("\nGrille de départ :\n");
    afficher_grille(&grille_init);

    printf("\n-----------------------------\n");

    // AJOUT DE LA GRILLE DE FIN
    char fgoal[] = "goal.txt";
    struct Grille grille_goal;
    grille_load(&grille_goal, fgoal);
    printf("\nGrille d'arrivée :\n");
    afficher_grille(&grille_goal);

    printf("\nRésolution en cours...\n");
    struct Pile* pile = creer_pile();
    pile = solve(&grille_init, &grille_goal);
}