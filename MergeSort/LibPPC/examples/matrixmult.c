#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include <libppc.h>

#define NLINES 3
#define NCOLS 3

int main(int argc, char ** argv){

	srand( time(NULL) );

	int matrix1_is_loaded = 0,
		matrix2_is_loaded = 0;

	/**
	 * 
	 * Matrizes em C precisam ser alocadas como vetores 
	 * acessadas pela regra M(i,j) = M[ i* colunas + j ]
	 * 
	 * */
	double *m1, *m2, *mR;

	if (access("m1.dat", F_OK) == 0) {
		printf("\nLoading Matrix 1 from file ...");

		m1 = load_double_matrix("m1.dat", NLINES, NCOLS);

		matrix1_is_loaded = 1;

	} else {

		printf("\nGenerating new Matrix 1 values...");
		m1 = (double*)generate_random_double_matrix( NLINES, NCOLS );
	
	}

	print_double_matrix( m1 , 3, 3);

	if (access("m2.dat", F_OK) == 0) {
		printf("\nLoading Matrix 2 from file ...");

		m2 = load_double_matrix("m2.dat", NLINES, NCOLS);

		matrix2_is_loaded = 1;

	} else {

		printf("\nGenerating new Matrix 2 values...");
		m2 = (double*)generate_random_double_matrix( NLINES, NCOLS );
	
	}

	print_double_matrix( m2 , 3, 3);

	mR = (double*)malloc(
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

	printf("\nResulting matrix multiplication:");
	print_double_matrix( mR , 3, 3);

	save_double_matrix( m1, NLINES, NCOLS, "m1.dat");
	save_double_matrix( m2, NLINES, NCOLS, "m2.dat");

	if (access("mR_1.dat", F_OK) == 0) {

		// There is already a result file, save current result and compare
		save_double_matrix( mR, NLINES, NCOLS, "mR_2.dat");

		int matrixes_are_equal = compare_double_matrixes_on_files("mR_1.dat", 
			"mR_2.dat",
			NLINES,
			NCOLS);
		
		if ( matrixes_are_equal ){
			printf("\nOK! Files are equal!");
		} else {
			printf("\nERROR! Files are NOT equal!");
		}

	} else {
		/*
		Save result for further comparison
		*/
		save_double_matrix( mR, NLINES, NCOLS, "mR_1.dat");
	}

	free( m1 );
	free( m2 );
	free( mR );

	return 0;
}