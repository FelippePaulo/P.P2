#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libppc.h>
#include <omp.h>
#include <string.h>

#define SIZE 400000

// Descomente para debug
//#define __DEBUG__

enum implementations_enum {
    TYPE_SERIAL = 1,
    TYPE_PARALLEL
};

void merge(double *array, long int left, long int mid, long int right) {
    long int n1 = mid - left + 1;
    long int n2 = right - mid;
    double *L = (double*)malloc(n1 * sizeof(double));
    double *R = (double*)malloc(n2 * sizeof(double));
    for (long int i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (long int j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];
    long int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) array[k++] = L[i++];
        else array[k++] = R[j++];
    }
    while (i < n1) array[k++] = L[i++];
    while (j < n2) array[k++] = R[j++];
    free(L);
    free(R);
}

void MergeSort_serial(double *array, long int left, long int right) {
    if (left < right) {
        long int mid = left + (right - left) / 2;
        MergeSort_serial(array, left, mid);
        MergeSort_serial(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void MergeSort_parallel(double *array, long int left, long int right, int depth) {
    if (left < right) {
        long int mid = left + (right - left) / 2;
        if (depth <= 0) {
            MergeSort_serial(array, left, mid);
            MergeSort_serial(array, mid + 1, right);
        } else {
#pragma omp parallel sections
            {
#pragma omp section
                MergeSort_parallel(array, left, mid, depth - 1);
#pragma omp section
                MergeSort_parallel(array, mid + 1, right, depth - 1);
            }
        }
        merge(array, left, mid, right);
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));
    double *vector_serial, *vector_2, *vector_4;
    if (access("vector.dat", F_OK) != 0) {
        printf("\nGenerating new vector...");
        vector_serial = generate_random_double_vector(SIZE, 0.0, 1000.0);
        save_double_vector(vector_serial, SIZE, "vector.dat");
    } else {
        printf("\nLoading vector from file...");
        vector_serial = load_double_vector("vector.dat", SIZE);
    }
    vector_2 = (double*)malloc(sizeof(double) * SIZE);
    vector_4 = (double*)malloc(sizeof(double) * SIZE);
    memcpy(vector_2, vector_serial, sizeof(double) * SIZE);
    memcpy(vector_4, vector_serial, sizeof(double) * SIZE);

    double start, end, time_serial = 0, time_parallel_2 = 0, time_parallel_4 = 0;
    printf("\nRunning serial MergeSort...");
    start = omp_get_wtime();
    MergeSort_serial(vector_serial, 0, SIZE - 1);
    end = omp_get_wtime();
    time_serial = end - start;
    printf("\nSerial time: %.6f seconds\n", time_serial);
    save_double_vector(vector_serial, SIZE, "sorted_serial.dat");

    printf("\n----------------------------------------------\n");

    omp_set_num_threads(2);
    printf("\nRunning parallel MergeSort (2 threads)...");
    start = omp_get_wtime();
    MergeSort_parallel(vector_2, 0, SIZE - 1, 1); // depth=1 para 2 threads
    end = omp_get_wtime();
    time_parallel_2 = end - start;
    printf("\nParallel time (2 threads): %.6f seconds\n", time_parallel_2);
    save_double_vector(vector_2, SIZE, "sorted_parallel_2.dat");
    double speedup_2 = time_serial / time_parallel_2;
    double eficiencia_2 = speedup_2 / 2.0;
    printf("\nSpeedup (2 threads): %.3f", speedup_2);
    printf("\nEficiência (2 threads): %.3f", eficiencia_2);

    printf("\n----------------------------------------------\n");

    omp_set_num_threads(4);
    printf("\nRunning parallel MergeSort (4 threads)...");
    start = omp_get_wtime();
    MergeSort_parallel(vector_4, 0, SIZE - 1, 2); // depth=2 para 4 threads
    end = omp_get_wtime();
    time_parallel_4 = end - start;
    printf("\nParallel time (4 threads): %.6f seconds\n", time_parallel_4);
    save_double_vector(vector_4, SIZE, "sorted_parallel_4.dat");
    double speedup_4 = time_serial / time_parallel_4;
    double eficiencia_4 = speedup_4 / 4.0;
    printf("\nSpeedup (4 threads): %.3f", speedup_4);
    printf("\nEficiência (4 threads): %.3f", eficiencia_4);

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
