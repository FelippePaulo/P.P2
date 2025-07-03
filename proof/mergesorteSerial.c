#include <stdio.h>
#include <stdlib.h>

void print_array(double *array, long int size) {
    for (long int i = 0; i < size; i++) {
        printf("%.2lf ", array[i]);
    }
    printf("\n");
}

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
    // Exibe o array após cada merge
    printf("Array após merge [%ld, %ld, %ld]: ", left, mid, right);
    print_array(array, right + 1); // ou o tamanho total do array
}

void MergeSort_serial(double *array, long int left, long int right) {
    if (left < right) {
        long int mid = left + (right - left) / 2;
        MergeSort_serial(array, left, mid);
        MergeSort_serial(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

int main() {
    double array[] = {5, 2, 9, 1, 6, 3};
    long int size = sizeof(array) / sizeof(array[0]);

    printf("Array inicial: ");
    print_array(array, size);

    MergeSort_serial(array, 0, size - 1);

    printf("Array final ordenado: ");
    print_array(array, size);

    return 0;
}