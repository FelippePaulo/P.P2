#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libppc.h>
#include <omp.h>
#include <string.h>
#include <stdbool.h>

// Defina aqui o tamanho do vetor
#define SIZE 10000

// Descomente para debug
//#define __DEBUG__

enum implementations_enum {
    TYPE_SERIAL = 1,
    TYPE_PARALLEL
};

void BubbleSort_serial(double *array, long int size) {
    int swapped;
    long int n = size;

    do {
        swapped = 0;

        for (long int i = 0; i < n - 1; i++) {
            // Dependência de dados:
            // A comparação e possível troca de array[i] e array[i+1]
            // afeta diretamente a próxima iteração do loop.
            if (array[i] > array[i + 1]) {
                // Região crítica (se paralelizado):
                // Troca de elementos compartilhados entre iterações.
                double temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;

                swapped = 1;
            }
        }

        n--; // Redução do limite de iteração a cada passo

    } while (swapped);
}


void BubbleSort_parallel(double *array, long int size) {
    bool swapped;
    do {
        swapped = false;

        // Fase 1: percorre os pares
        // Cada thread cuida de um par distinto, então não há risco de duas threads
        // acessarem ou alterarem as mesmas posições.
        // O uso de 'reduction' garante que a variável 'swapped' seja atualizada corretamente.
        #pragma omp parallel for shared(array) reduction(||:swapped) schedule(static)
        for (long int i = 0; i < size - 1; i += 2) {
            if (array[i] > array[i + 1]) {
                double temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;
                swapped = true;
            }
        }

        // Fase 2: percorre os pares começando do índice ímpar (1, 3, 5, ...)
        // Mesmo esquema da fase anterior, mas deslocado para cobrir os pares que ficaram de fora.
        #pragma omp parallel for shared(array) reduction(||:swapped) schedule(static)
        for (long int i = 1; i < size - 1; i += 2) {
            if (array[i] > array[i + 1]) {
                double temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;
                swapped = true;
            }
        }

        // Observação: o loop externo (do-while) **não pode** ser paralelizado,
        // pois o estado do vetor depende das trocas feitas na iteração anterior.
        // Ou seja, existe uma dependência de dados entre as rodadas do loop principal.
    } while (swapped);
}



int main(int argc, char **argv) {
    srand(time(NULL));

    // Sempre gere ou carregue o vetor original
    double *vector_serial, *vector_2, *vector_4;
    if (access("vector.dat", F_OK) != 0) {
        printf("\nGenerating new vector...");
        vector_serial = generate_random_double_vector(SIZE, 0.0, 1000.0);
        save_double_vector(vector_serial, SIZE, "vector.dat");
    } else {
        printf("\nLoading vector from file...");
        vector_serial = load_double_vector("vector.dat", SIZE);
    }
    // Cópias para execuções paralelas
    vector_2 = (double*)malloc(sizeof(double) * SIZE);
    vector_4 = (double*)malloc(sizeof(double) * SIZE);
    memcpy(vector_2, vector_serial, sizeof(double) * SIZE);
    memcpy(vector_4, vector_serial, sizeof(double) * SIZE);

    // Serial
    double start, end, time_serial = 0, time_parallel_2 = 0, time_parallel_4 = 0;
    printf("\nRunning serial Bubblesort...");
    start = omp_get_wtime();
    BubbleSort_serial(vector_serial, SIZE);
    end = omp_get_wtime();
    time_serial = end - start;
    printf("\nSerial time: %.6f seconds\n", time_serial);
    save_double_vector(vector_serial, SIZE, "sorted_serial.dat");

     printf("\n----------------------------------------------\n");

    // Paralelo 2 threads
    omp_set_num_threads(2);
    printf("\nRunning parallel Bubblesort (2 threads)...");
    start = omp_get_wtime();
    BubbleSort_parallel(vector_2, SIZE);
    end = omp_get_wtime();
    time_parallel_2 = end - start;
    printf("\nParallel time (2 threads): %.6f seconds\n", time_parallel_2);
    save_double_vector(vector_2, SIZE, "sorted_parallel_2.dat");
    double speedup_2 = time_serial / time_parallel_2;
    double eficiencia_2 = speedup_2 / 2.0;
    printf("\nSpeedup (2 threads): %.3f", speedup_2);
    printf("\nEficiência (2 threads): %.3f", eficiencia_2);

    printf("\n----------------------------------------------\n");

    // Paralelo 4 threads
    omp_set_num_threads(4);
    printf("\nRunning parallel Bubblesort (4 threads)...");
    start = omp_get_wtime();
    BubbleSort_parallel(vector_4, SIZE);
    end = omp_get_wtime();
    time_parallel_4 = end - start;
    printf("\nParallel time (4 threads): %.6f seconds\n", time_parallel_4);
    save_double_vector(vector_4, SIZE, "sorted_parallel_4.dat");
    double speedup_4 = time_serial / time_parallel_4;
    double eficiencia_4 = speedup_4 / 4.0;
    printf("\nSpeedup (4 threads): %.3f", speedup_4);
    printf("\nEficiência (4 threads): %.3f", eficiencia_4);

    // Comparação dos resultados
    printf("\nComparing parallel results with serial...");
    int vectors_are_equal_2 = compare_double_vector_on_files(
        "sorted_serial.dat",
        "sorted_parallel_2.dat"
    );
    if (vectors_are_equal_2) {
        printf("\nOK! Serial and parallel (2 threads) outputs are equal!");
    } else {
        printf("\nERROR! Outputs are NOT equal for 2 threads!");
    }
    int vectors_are_equal_4 = compare_double_vector_on_files(
        "sorted_serial.dat",
        "sorted_parallel_4.dat"
    );
    if (vectors_are_equal_4) {
        printf("\nOK! Serial and parallel (4 threads) outputs are equal!");
    } else {
        printf("\nERROR! Outputs are NOT equal for 4 threads!");
    }

    free(vector_serial);
    free(vector_2);
    free(vector_4);
    printf("\n");
    return 0;
}
