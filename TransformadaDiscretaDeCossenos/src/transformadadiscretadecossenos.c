#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libppc.h>
#include <omp.h>
#include <math.h>
#include <string.h>

#define SIZE 200000
#define PI 3.14159265358979323846

enum implementations_enum {
    TYPE_SERIAL = 1,
    TYPE_PARALLEL
};

void DCT1D_serial(const double *input, double *output, long int N) {
    double ck = 1.0;

    for (long int k = 0; k < N; k++) {
        // ck depende apenas do índice k, não há risco de condição de corrida
        if (k > 0) ck = sqrt(2.0/N);
        else ck = sqrt(1.0/N);

        double sum = 0.0;

        // Dependência de dados: o cálculo de 'sum' depende de input[n] e de k,
        // mas não há sobrescrita nem leitura concorrente. A variável 'sum' é local.
        for (long int n = 0; n < N; n++) {
            sum += input[n] * cos(PI * (n + 0.5) * k / N);
        }

        // Escrita em output[k], única para cada iteração.
        // Como a execução é sequencial, não existe região crítica.
        output[k] = ck * sum;
    }
}



void DCT1D_parallel(const double *input, double *output, long int N) {
    // Cada iteração calcula um valor exclusivo de output[k].
    #pragma omp parallel for
    for (long int k = 0; k < N; k++) {
        double ck = (k == 0) ? sqrt(1.0/N) : sqrt(2.0/N);

        double sum = 0.0;
        // Dependência de dados dentro da variável local 'sum'.
        // Não há conflito, pois cada thread possui sua própria cópia dessa variável.
        for (long int n = 0; n < N; n++) {
            sum += input[n] * cos(PI * (n + 0.5) * k / N);
        }

        output[k] = ck * sum;
    }
}


int main(int argc, char **argv) {
    srand(time(NULL));
    double *vector, *output_serial, *output_2, *output_4;
    if (access("vector.dat", F_OK) != 0) {
        printf("\nGenerating new vector...");
        vector = generate_random_double_vector(SIZE, 0.0, 1000.0);
        save_double_vector(vector, SIZE, "vector.dat");
    } else {
        printf("\nLoading vector from file...");
        vector = load_double_vector("vector.dat", SIZE);
    }
    output_serial = (double*)malloc(sizeof(double) * SIZE);
    output_2 = (double*)malloc(sizeof(double) * SIZE);
    output_4 = (double*)malloc(sizeof(double) * SIZE);

    double start, end, time_serial = 0, time_parallel_2 = 0, time_parallel_4 = 0;
    printf("\nRunning serial DCT 1D...");
    start = omp_get_wtime();
    DCT1D_serial(vector, output_serial, SIZE);
    end = omp_get_wtime();
    time_serial = end - start;
    printf("\nSerial time: %.6f seconds\n", time_serial);
    save_double_vector(output_serial, SIZE, "dct_serial.dat");

    printf("\n----------------------------------------------\n");

    omp_set_num_threads(2);
    printf("\nRunning parallel DCT 1D (2 threads)...");
    start = omp_get_wtime();
    DCT1D_parallel(vector, output_2, SIZE);
    end = omp_get_wtime();
    time_parallel_2 = end - start;
    printf("\nParallel time (2 threads): %.6f seconds\n", time_parallel_2);
    save_double_vector(output_2, SIZE, "dct_parallel_2.dat");
    double speedup_2 = time_serial / time_parallel_2;
    double eficiencia_2 = speedup_2 / 2.0;
    printf("\nSpeedup (2 threads): %.3f", speedup_2);
    printf("\nEficiência (2 threads): %.3f", eficiencia_2);

    printf("\n----------------------------------------------\n");

    omp_set_num_threads(4);
    printf("\nRunning parallel DCT 1D (4 threads)...");
    start = omp_get_wtime();
    DCT1D_parallel(vector, output_4, SIZE);
    end = omp_get_wtime();
    time_parallel_4 = end - start;
    printf("\nParallel time (4 threads): %.6f seconds\n", time_parallel_4);
    save_double_vector(output_4, SIZE, "dct_parallel_4.dat");
    double speedup_4 = time_serial / time_parallel_4;
    double eficiencia_4 = speedup_4 / 4.0;
    printf("\nSpeedup (4 threads): %.3f", speedup_4);
    printf("\nEficiência (4 threads): %.3f", eficiencia_4);

    printf("\nComparing parallel results with serial...");
    int vectors_are_equal_2 = compare_double_vector_on_files(
        "dct_serial.dat",
        "dct_parallel_2.dat"
    );
    if (vectors_are_equal_2) {
        printf("\nOK! Serial and parallel (2 threads) outputs are equal!");
    } else {
        printf("\nERROR! Outputs are NOT equal for 2 threads!");
    }
    int vectors_are_equal_4 = compare_double_vector_on_files(
        "dct_serial.dat",
        "dct_parallel_4.dat"
    );
    if (vectors_are_equal_4) {
        printf("\nOK! Serial and parallel (4 threads) outputs are equal!");
    } else {
        printf("\nERROR! Outputs are NOT equal for 4 threads!");
    }

    free(vector);
    free(output_serial);
    free(output_2);
    free(output_4);
    printf("\n");
    return 0;
}
