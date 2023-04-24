#include <stdio.h>

/*
                            << UTILITIES.C >>
    Fichier annexe avec des fonctions de traitement de vecteur, non relaté aux structures du jeu :
    - swap(int* a, int* b)
    - partition(int arr[], int low, int high)
    - quicksort(int arr[], int low, int high)
    - special_sort(int arr[], int low, int high)

    - DEBUG : print_arr(int arr[],int n)
*/

// Fonction pour échanger deux éléments
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
// Fonction pour partitionner le tableau et retourner l'index du pivot
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Fonction pour trier le tableau en utilisant QuickSort
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

//Utilistation du Quicksort + Agencement du Zéro à la fin de la liste 
void special_sort(int arr[], int low, int high) {
    quicksort(arr, low, high-1);
    int temp_arr[high];
    for (int i = 1; i < high; i++)
    {
        temp_arr[i-1] = arr[i];
    }
    for (int i = 0; i < high; i++)
    {
        arr[i] = temp_arr[i];
    }
}

void print_arr(int arr[],int n){
    for (int i = 0; i < n; i++) {
        printf(" arr[%d] : %d ",i, arr[i]);
    }
}