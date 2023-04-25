#include "main.c"

/*
                            << PILE.C >>
    Fichiers annexe avec la structure d'un noeud, d'une pile et leurs fonctions :

    - creer_pile()
    - sommet(struct Pile* pile) 
    - empiler(struct Pile* pile, struct Grille* grille)
    - depiler(struct Pile* pile)
    - dans_la_pile(struct Pile* pile, struct Grille* grille)
    - is_empty(struct Pile* pile)

    _ Node
    _ Pile
*/

struct Node {
    struct Grille* grille;
    struct Node* next;
};

struct Pile {
    struct Node* top;
};

// Fonction qui crée une pile vide
struct Pile* creer_pile() {
    struct Pile* pile = malloc(sizeof(struct Pile));
    pile->top = NULL;
    return pile;
}

struct Grille* sommet(struct Pile* pile) {
    if (pile->top == NULL)
        return NULL;
    else
        return pile->top->grille;
}


// Fonction qui ajoute une grille au sommet de la pile
void empiler(struct Pile* pile, struct Grille* grille) {
    struct Node* new_node = malloc(sizeof(struct Node));
    new_node->grille = grille;
    new_node->next = pile->top;
    pile->top = new_node;
}

// Fonction qui supprime l'élément au sommet de la pile et renvoie la grille correspondante
void depiler(struct Pile* pile) {
    if (pile->top == NULL) {
    }
    else
    {
        struct Node* top_node = pile->top;
        struct Grille* grille = top_node->grille;
        pile->top = top_node->next;
        free(top_node);
    }
}

// Fonction qui vérifie si une grille est dans la pile
bool dans_la_pile(struct Pile* pile, struct Grille* grille) {
    struct Node* current = pile->top;
    while (current != NULL) {
        if (current->grille == grille) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool is_empty(struct Pile* pile) {
    return pile->top == NULL;
}