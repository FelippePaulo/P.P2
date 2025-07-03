#include <unistd.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include <libppc.h>

#include <omp.h>

#define NLINES 1000
#define NCOLS 1000

// Descomente esta linha abaixo para imprimir valores das matrizes 
//#define __DEBUG__

enum implementations_enum {
	TYPE_SERIAL = 1, 
	TYPE_PARALLEL
} ;

double *MatrixMult_serial(const double *m1, const double *m2){

	double *mR = (double*)malloc(
		sizeof(double) * NLINES * NCOLS);

	for ( long int i = 0; i < NLINES; i++ ){

		for ( long int j = 0; j < NCOLS; j++ ){

			// Utilize esta macro abaixo para acessar valores de matrizes em C
			M( i , j , NCOLS, mR) = 0;

			for (long int k = 0; k < NCOLS; k++ ){

				M(i, j, NCOLS, mR) += 
					M( i , k , NCOLS, m1) * M( k , j , NCOLS, m2);

			}

		}
	}
	
	return mR;
}


double *MatrixMult_parallel(const double *m1, const double *m2){

	double *mR = (double*)malloc(
		sizeof(double) * NLINES * NCOLS);

	#pragma omp parallel for collapse(2)
	for ( long int i = 0; i < NLINES; i++ ){
		for ( long int j = 0; j < NCOLS; j++ ){
			
			M( i , j , NCOLS, mR) = 0;
			for (long int k = 0; k < NCOLS; k++ ){
				M(i, j, NCOLS, mR) += 
					M( i , k , NCOLS, m1) * M( k , j , NCOLS, m2);
			}
		}
	}
	return mR;
}


int main(int argc, char ** argv){
    srand( time(NULL) );
    double *m1, *m2, *mR_serial, *mR_2, *mR_4;
    if (access("m1.dat", F_OK) != 0) {
        printf("\nGenerating new Matrix 1 values...");
        m1 = (double*)generate_random_double_matrix( NLINES, NCOLS );
        save_double_matrix( m1, NLINES, NCOLS, "m1.dat");
    } else {
        printf("\nLoading Matrix 1 from file ...");
        m1 = load_double_matrix("m1.dat", NLINES, NCOLS);
    }
    if (access("m2.dat", F_OK) != 0) {
        printf("\nGenerating new Matrix 2 values...");
        m2 = (double*)generate_random_double_matrix( NLINES, NCOLS );
        save_double_matrix( m2, NLINES, NCOLS, "m2.dat");
    } else {
        printf("\nLoading Matrix 2 from file ...");
        m2 = load_double_matrix("m2.dat", NLINES, NCOLS);
    }
    double start, end, time_serial = 0, time_parallel_2 = 0, time_parallel_4 = 0;
    printf("\nRunning serial implementation ...");
    start = omp_get_wtime();
    mR_serial = MatrixMult_serial(m1, m2);
    end = omp_get_wtime();
    time_serial = end - start;
    printf("\nSerial implementation took %.6f seconds", time_serial);
    save_double_matrix(mR_serial, NLINES, NCOLS, "mR_serial.dat");

    printf("\n----------------------------------------------\n");
    omp_set_num_threads(2);
    printf("\nRunning parallel implementation (2 threads) ...");
    start = omp_get_wtime();
    mR_2 = MatrixMult_parallel(m1, m2);
    end = omp_get_wtime();
    time_parallel_2 = end - start;
    printf("\nParallel implementation took %.6f seconds (2 threads)", time_parallel_2);
    save_double_matrix(mR_2, NLINES, NCOLS, "mR_parallel_2.dat");
    double speedup_2 = time_serial / time_parallel_2;
    double eficiencia_2 = speedup_2 / 2.0;
    printf("\nSpeedup (2 threads): %.3f", speedup_2);
    printf("\nEficiência (2 threads): %.3f", eficiencia_2);

    printf("\n----------------------------------------------\n");
    omp_set_num_threads(4);
    printf("\nRunning parallel implementation (4 threads) ...");
    start = omp_get_wtime();
    mR_4 = MatrixMult_parallel(m1, m2);
    end = omp_get_wtime();
    time_parallel_4 = end - start;
    printf("\nParallel implementation took %.6f seconds (4 threads)", time_parallel_4);
    save_double_matrix(mR_4, NLINES, NCOLS, "mR_parallel_4.dat");
    double speedup_4 = time_serial / time_parallel_4;
    double eficiencia_4 = speedup_4 / 4.0;
    printf("\nSpeedup (4 threads): %.3f", speedup_4);
    printf("\nEficiência (4 threads): %.3f", eficiencia_4);

    printf("\nComparing parallel results with serial...");
    int matrixes_are_equal_2 = compare_double_matrixes_on_files(
        "mR_serial.dat",
        "mR_parallel_2.dat",
        NLINES,
        NCOLS
    );
    if (matrixes_are_equal_2) {
        printf("\nOK! Serial and parallel (2 threads) outputs are equal!");
    } else {
        printf("\nERROR! Outputs are NOT equal for 2 threads!");
    }
    int matrixes_are_equal_4 = compare_double_matrixes_on_files(
        "mR_serial.dat",
        "mR_parallel_4.dat",
        NLINES,
        NCOLS
    );
    if (matrixes_are_equal_4) {
        printf("\nOK! Serial and parallel (4 threads) outputs are equal!");
    } else {
        printf("\nERROR! Outputs are NOT equal for 4 threads!");
    }

    free(m1);
    free(m2);
    free(mR_serial);
    free(mR_2);
    free(mR_4);
    printf("\n");
    return 0;
}
