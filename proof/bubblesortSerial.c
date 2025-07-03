#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void BubbleSort_serial(double *array, long int size) {
    int swapped;
    long int n = size;
    int pass = 1;
    
    do {
        swapped = 0;
        printf("\n--- Passagem %d ---\n", pass++);
        
        for (long int i = 0; i < n - 1; i++) {
            if (array[i] > array[i + 1]) {
                // Mostra o array antes da inversão
                printf("Antes da inversão:  [");
                for (long int j = 0; j < size; j++) {
                    if (j == i || j == i+1) {
                        printf("\033[1;31m%.2f\033[0m", array[j]); // Vermelho para os elementos a serem trocados
                    } else {
                        printf("%.2f", array[j]);
                    }
                    if (j < size-1) printf(", ");
                }
                printf("]\n");
                
                // Faz a troca
                double temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;
                swapped = 1;
                
                // Mostra o array após a inversão
                printf("Após a inversão:   [");
                for (long int j = 0; j < size; j++) {
                    if (j == i || j == i+1) {
                        printf("\033[1;32m%.2f\033[0m", array[j]); // Verde para os elementos trocados
                    } else {
                        printf("%.2f", array[j]);
                    }
                    if (j < size-1) printf(", ");
                }
                printf("]\n\n");
            }
        }
        n--; // Reduz o limite a cada passagem
        
        // Mostra o array após cada passagem completa
        printf("Array após passagem: [");
        for (long int j = 0; j < size; j++) {
            printf("%.2f", array[j]);
            if (j < size-1) printf(", ");
        }
        printf("]\n");
        
    } while (swapped);
}

int main() {
    // Configura a semente para números aleatórios
    srand(time(NULL));
    
    // Tamanho do vetor (reduzido para facilitar a visualização)
    long int size = 6;
    double *array = (double *)malloc(size * sizeof(double));
    
    // Preenche o vetor com valores aleatórios entre 0 e 100
    printf("Vetor original:\n[");
    for (long int i = 0; i < size; i++) {
        array[i] = (double)(rand() % 1000) / 10.0; // Valores entre 0.0 e 99.9
        printf("%.2f", array[i]);
        if (i < size-1) printf(", ");
    }
    printf("]\n\n");
    
    // Chama a função de ordenação
    printf("Iniciando ordenação...\n");
    BubbleSort_serial(array, size);
    
    // Imprime o vetor ordenado
    printf("\nVetor final ordenado:\n[");
    for (long int i = 0; i < size; i++) {
        printf("%.2f", array[i]);
        if (i < size-1) printf(", ");
    }
    printf("]\n");
    
    // Libera a memória
    free(array);
    
    return 0;
}