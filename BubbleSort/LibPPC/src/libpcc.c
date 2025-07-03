

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include <stdlib.h>

#include <libppc.h>

void print_double_vector(const double *data, long int size, long int line_break){

	long int i, j;

	for ( i = 0; i < size; i++ ){
		fprintf(stdout,"\ndata[ %ld ] = %.6f ", i, data[i]);

		if ( i % line_break == 0 && i>0){
			fprintf(stdout,"\n");
		}
	}

	fprintf(stdout,"\n");
}


void print_double_complex_vector(const double complex *data, long int size, long int line_break)
{

	long int i, j;

	for ( i = 0; i < size; i++ ){
	
		fprintf(stderr,"\ndata[ %ld ] = %.3f + %.3fi", 
			i, 
			creal(data[i]),
			cimag(data[i]));

		if ( i % line_break == 0 && i>0){
			fprintf(stderr,"\n");
		}
		
	}

	fprintf(stdout,"\n");
}



void print_int_vector(const int* data, long int size, long int line_break)
{

	long int i, j;

	for ( i = 0; i < size; i++ ){
	
		fprintf(stderr,"\ndata[ %ld ] = %d", i, data[i] );

		if ( i % line_break == 0 && i>0){
			fprintf(stderr,"\n");
		}
		
	}

	fprintf(stdout,"\n");
}


int save_double_vector(const double *data, long int size, const char *filename ){

	FILE *fd = NULL;
	
	fd = fopen( filename , "wb" );

	long int nbytes = fwrite( data , sizeof(double), size , fd );

	if ( nbytes != size ) {

		fclose( fd );

		fprintf(stderr, "Error: saved size (%ld) is not the requested size (%ld)",
			nbytes,
			size );

		return nbytes;

	} else {

		fclose( fd );

		return 0;
	}
}


int save_double_complex_vector(const double complex *data, long int size, const char *filename ){

	FILE *fd = NULL;
	
	fd = fopen( filename , "wb" );

	long int nbytes = fwrite( data , sizeof(double complex), size , fd );

	if ( nbytes != size ) {

		fclose( fd );

		fprintf(stderr, "Error: saved size (%ld) is not the requested size (%ld)",
			nbytes,
			size );

		return nbytes;

	} else {

		fclose( fd );

		return 0;

	}
}


int save_int_vector(const int *data, long int size, const char *filename ){

	FILE *fd = NULL;
	
	fd = fopen( filename , "wb" );

	long int nbytes = fwrite( data , sizeof(int), size , fd );

	if ( nbytes != size ) {

		fclose( fd );

		fprintf(stderr, "Error: saved size (%ld) is not the requested size (%ld)",
			nbytes,
			size );

		return nbytes;

	} else {

		fclose( fd );

		return 0;

	}
}


double* load_double_vector(const char *filename, long int size){

	FILE *fd = NULL;

	double *data = (double*)malloc(sizeof(double)*size);
	
	fd = fopen( filename , "rb" );

	int nbytes = fread( data , sizeof(double), size, fd );

	if ( nbytes == size ){

		fclose( fd );
		
		return data;

	} else {

		fprintf(stderr, "Error: requested vector size (%ld) is not the size read (%d)",
			size,
			nbytes);

		free(data);

		fclose( fd );

		return NULL;

	}
}



int* load_int_vector(const char *filename, long int size){

	FILE *fd = NULL;

	int *data = (int*)malloc(sizeof(int)*size);
	
	fd = fopen( filename , "rb" );

	int nbytes = fread( data , sizeof(int), size, fd );

	if ( nbytes == size ){

		fclose( fd );
		
		return data;

	} else {

		fprintf(stderr, "Error: requested vector size (%ld) is not the size read from file (%d)",
			size,
			nbytes);

		free(data);

		fclose( fd );

		return NULL;

	}
}



double* generate_random_double_vector(long int quantity, double minvalue, double maxvalue)
{
	srand( time(NULL) );

	double *vector = (double*)malloc( sizeof(double)*quantity );
		
	double step_range = ( ( maxvalue - minvalue ) / ( (double) RAND_MAX ) );

	for ( unsigned long i = 0; i< quantity; i++){
		
		double num = ( ( (double) rand() ) * step_range ) - minvalue;

		vector[ i ] = num;

	}	

	return vector;

}


int* generate_random_int_vector(long int quantity, int minvalue, int maxvalue)
{
	srand( time(NULL) );

	int *vector = (int*)malloc( sizeof(int)*quantity );
		
	double step_range = ( ( maxvalue - minvalue ) / ( (double) RAND_MAX ) );

	for ( unsigned long i = 0; i< quantity; i++){
		
		double num = ( ( (double) rand() ) * step_range ) - minvalue ;

		vector[ i ] = (int)num;

	}	

	return vector;

}



point2D_t* generate_random_2Dpoints_vector(long int quantity, double minvalue, double maxvalue)
{
	srand( time(NULL) );

	point2D_t *vector = (point2D_t*)malloc( sizeof( point2D_t )*quantity );
		
	double step_range = ( ( maxvalue - minvalue ) / ( (double) RAND_MAX ) );

	for ( long int i = 0; i< quantity; i++){
		
		double x = ( ( (double) rand() ) * step_range ) - minvalue ;
		
		double y = ( ( (double) rand() ) * step_range ) - minvalue ;

		vector[ i ].x = x;
		vector[ i ].y = y;

	}	

	return vector;

}



int compare_double_vectors( 
	const double *vector1,
	const double *vector2,
	long int size)
{

	for ( long int i = 0; i < size; i ++ ) {

		if ( vector1[ i ] != vector2[ i ]){

			return 1;

		}
	}

	return 0;
}


int compare_int_vectors_on_files(const char *vector_file1, const char *vector_file2)
{
	FILE *vector1_fp, *vector2_fp;

	vector1_fp = fopen(vector_file1, "r");
	vector2_fp = fopen(vector_file2, "r");

	// Buffers are 512 bytes to avoid memory overflow
	int *file1_buffer = (int*)malloc(  sizeof(int) * 512 );

	int *file2_buffer = (int*)malloc(  sizeof(int) * 512 );

	size_t nbytes_read_file1, nbytes_read_file2;

	// Default return is file are equal
	int files_are_equal = 1;

	while ( nbytes_read_file1 = fread( file1_buffer, sizeof(int) , 512 , vector1_fp ) ) {

		nbytes_read_file2 = fread( file2_buffer, sizeof(int) , 512 , vector2_fp );

		if ( nbytes_read_file1 != nbytes_read_file2 ){
			// Files have different number of elements, return false
			files_are_equal = 0;
			break;
		}

		// Check if elements are the same
		for ( long int i = 0; i < nbytes_read_file1 ; i ++ ){

			if ( file1_buffer[ i ] != file2_buffer[ i ] ){

				files_are_equal = 0;
				break;

			}

		}

		if (! files_are_equal ){
			break;
		}

	}

	free( file1_buffer );
	free( file2_buffer );

	fclose( vector1_fp );
	fclose( vector2_fp );

	return files_are_equal;	
}



int compare_double_vector_on_files(const char *vector_file1, const char *vector_file2)
{
	FILE *vector1_fp, *vector2_fp;

	vector1_fp = fopen(vector_file1, "r");
	vector2_fp = fopen(vector_file2, "r");

	// Buffers are 512 bytes to avoid memory overflow
	double *file1_buffer = (double*)malloc(  sizeof(double) * 512 );

	double *file2_buffer = (double*)malloc(  sizeof(double) * 512 );

	size_t nbytes_read_file1, nbytes_read_file2;

	// Default return is file are equal
	int files_are_equal = 1;

	while ( nbytes_read_file1 = fread( file1_buffer, sizeof(double) , 512 , vector1_fp ) ) {

		nbytes_read_file2 = fread( file2_buffer, sizeof(double) , 512 , vector2_fp );

		if ( nbytes_read_file1 != nbytes_read_file2 ){
			// Files have different number of elements, return false
			files_are_equal = 0;
			break;
		}

		// Check if elements are the same
		for ( long int i = 0; i < nbytes_read_file1 ; i ++ ){

			if ( file1_buffer[ i ] != file2_buffer[ i ] ){

				files_are_equal = 0;
				break;

			}

		}

		if (! files_are_equal ){
			break;
		}

	}

	free( file1_buffer );
	free( file2_buffer );

	fclose( vector1_fp );
	fclose( vector2_fp );

	return files_are_equal;	
}


int compare_double_complex_vector_on_files(const char *vector_file1, const char *vector_file2)
{
	FILE *vector1_fp, *vector2_fp;

	vector1_fp = fopen(vector_file1, "r");
	vector2_fp = fopen(vector_file2, "r");

	// Buffers are 512 bytes to avoid memory overflow
	double *file1_buffer = (double*)malloc(  sizeof(double complex) * 512 );

	double *file2_buffer = (double*)malloc(  sizeof(double complex) * 512 );

	size_t nbytes_read_file1, nbytes_read_file2;

	// Default return is file are equal
	int files_are_equal = 1;

	while ( nbytes_read_file1 = fread( file1_buffer, sizeof(double) , 512 , vector1_fp ) ) {

		nbytes_read_file2 = fread( file2_buffer, sizeof(double) , 512 , vector2_fp );

		if ( nbytes_read_file1 != nbytes_read_file2 ){
			// Files have different number of elements, return false
			files_are_equal = 0;
			break;
		}

		// Check if elements are the same
		for ( long int i = 0; i < nbytes_read_file1 ; i ++ ){

			if ( file1_buffer[ i ] != file2_buffer[ i ] ){

				files_are_equal = 0;
				break;

			}

		}

		if (! files_are_equal ){
			break;
		}

	}

	free( file1_buffer );
	free( file2_buffer );

	fclose( vector1_fp );
	fclose( vector2_fp );

	return files_are_equal;	
}


void print_double_matrix(double *matrix, 
	long int lines, 
	long int columns)
{
	long int i, j;

	fprintf(stdout, "\n");

	for ( i = 0; i < lines; i++ ){

		for ( j = 0; j < columns; j++ ){

			fprintf(stdout, "%.3lf", matrix[ i*columns + j ]  );

			if ( j < columns -1 ){
				fprintf(stdout, "\t");
			} else {
				fprintf(stdout, "\n");
			}

		}

		
	}
}


double* generate_random_double_matrix(
	long int lines, 
	long int columns)
{
	double *matrix = (double*)malloc( sizeof(double) * lines * columns );

	for ( long int i = 0; i < lines; i++ ){

		for ( long int j = 0; j < columns; j++ ){

			double x = rand() % ( lines * columns );

			matrix[ i * columns + j ] = x ;

		}		
	}

	return ((double*)matrix);
}



int compare_double_matrixes(const double **matrix1, 
	const double **matrix2, 
	long int lines,
	long int columns)
{
	long int i, j;

	for ( i = 0; i < lines; i++ ){
		for ( j = 0; j < columns; j++ ){

			if ( matrix1[ i ][ j ] != matrix2[ i ][ j ])
				return 0;
		}
	}

	return 1;
	
}


int save_double_matrix(const double *matrix, 
	long int number_of_lines, 
	long int number_of_columns,
	const char *filename )
{

	FILE *fd = NULL;

	size_t size = number_of_columns * number_of_lines;
	
	fd = fopen( filename , "wb" );

	int nbytes = fwrite( matrix , sizeof(double), size , fd );

	if ( nbytes != size ) {

		fclose( fd );

		perror("Error: size saved is not the size of the matrix");

		return nbytes;

	} else {

		fclose( fd );

		return 0;
	}
}

double* load_double_matrix( const char *filename, 
	long int number_of_lines,
	long int number_of_columns)
{

	FILE *fd = NULL;
	
	fd = fopen( filename , "r" );

	size_t size = number_of_lines * number_of_columns;

	double *matrix=(double*)malloc( size*sizeof(double) );

	int nbytes = fread( matrix , sizeof(double), size, fd );

	if ( nbytes == size ){

		fclose( fd );
		
		return matrix;

	} else {

		perror("Error: matrix size saved on file is not the requested by the function");

		fclose( fd );

		return NULL;

	}
}


int compare_double_matrixes_on_files(const char *matrix_file1, 
	const char *matrix_file2,
	long int number_of_lines,
	long int number_of_columns)
{

	FILE *matrix1_fp, *matrix2_fp;

	matrix1_fp = fopen( matrix_file1, "rb");
	if (matrix1_fp == NULL){
		printf("Error openning matrix file %s!", matrix_file1);
		return 0;
	}

	matrix2_fp = fopen( matrix_file2, "rb");
	if (matrix2_fp == NULL){
		printf("Error openning matrix file %s!", matrix_file2);
		return 0;
	}	

	long int line_size = sizeof(double) * number_of_columns;

	// printf("\nLine size = %ld", line_size);

	// Buffers are line-aligned to avoid big memory allocation
	double *file1_buffer = (double*) malloc( line_size );

	double *file2_buffer = (double*) malloc( line_size );

	size_t n_elements_read1, n_elements_read2;

	// Default return is file are equal
	int files_are_equal = 1;

	// counter for the number of lines read
	long int lines_read = 0;

	while ( n_elements_read1 = fread( file1_buffer, sizeof(double) , number_of_columns , matrix1_fp ) ) {
	
#ifdef __EXTRA_DEBUG_MESSAGES__	
		fprintf(stderr, "\nn_elements_read1 = %ld", n_elements_read1 );
#endif		

		if ( n_elements_read1 < number_of_columns){
				
			printf("Error! Number of elements read from matrix1 line (%ld) is different from line size! (%ld)",
				n_elements_read1,
				number_of_columns);

			files_are_equal = 0;
			break;
		}

		n_elements_read2 = fread( file2_buffer, sizeof(double) , number_of_columns , matrix2_fp );
		
#ifdef __EXTRA_DEBUG_MESSAGES__		
		fprintf(stderr, "\nn_elements_read2 = %ld", n_elements_read2 );
#endif		

		if ( n_elements_read2 < number_of_columns){
			printf("Error! Number of elements read (%ld) from matrix2 line is different from line size! (%ld)",
				n_elements_read2,
				number_of_columns);

			files_are_equal = 0;
			break;
		}

#if 0		
		if ( nbytes_read_file1 != nbytes_read_file2 ){
			// Files have different number of elements, return false
			printf("\nError! Read size is different from files!");
			files_are_equal = 0;
			break;
		}

		long int read_line_size1 = (nbytes_read_file1/sizeof(double));
		printf("%ld", read_line_size1);
		if ( read_line_size1 != line_size ){
			// Number of bytes read is less than line size, abort
			printf("\nError! Read line size from matrix1 (%ld) is not the expected (%ld)!",
				read_line_size1,
				line_size);

			files_are_equal = 0;
			break;
		}

		long int read_line_size2 = (nbytes_read_file2/sizeof(double));
		if ( read_line_size2 != line_size ){
			// Number of bytes read is less than line size, abort
			printf("\nError! Read line size from matrix2 (%ld) is not the expected (%ld)!",
				read_line_size2,
				line_size);
			files_are_equal = 0;
			break;
		}	
#endif	

		// Check if elements are the same
		for ( long int i = 0; i < number_of_columns ; i ++ ){

			if ( file1_buffer[ i ] != file2_buffer[ i ] ){
				printf("\nError! Different saved elements on matrix!");
				files_are_equal = 0;
				break;
			}

		}

		if (! files_are_equal ){
			break;
		}

		lines_read ++ ;

	}

	free( file1_buffer );
	free( file2_buffer );

	fclose( matrix1_fp );
	fclose( matrix2_fp );

	if (lines_read == number_of_lines )
		return files_are_equal;	
	else 
		return 0;

}




#if 0
void save_vector(DATA_T *data, size_t size, const char *filename){
	long int i,n_bytes;
	
	FILE *file = fopen(filename,"w");

	n_bytes=fwrite(data,sizeof(DATA_T),size,file);

	if (n_bytes!=size){
		fprintf(stderr,"Error saving matrix (%ld/%ld elements written)\n",n_bytes,size);
		exit(-1);
	}	

	fclose(file);
}

int compare_vector(DATA_T *data, size_t size, const char *filename){
	long int i,n_bytes;
	DATA_T tmp_data;	
	FILE *file = fopen(filename,"r");
	void *buffer=(void*)malloc(sizeof(DATA_T)*size);

	n_bytes=fread(buffer,sizeof(DATA_T),size,file);

	if (DEBUG_TESTS){
		
		int inject = rand()%10;
		if (inject%3==0){
			i = rand()%size;

			fprintf(stderr,"inverting bits at %ld...\n",i);

			tmp_data = !( ((DATA_T*)buffer)[i]  );

			((DATA_T*)buffer)[i]= tmp_data ;
		}
	}

	if (n_bytes!=size){
		fprintf(stderr,"Error loading saved matrix (%ld/%ld elements read)\n",n_bytes,size);
		exit(-1);
	}

	for ( i=0; i < size ; i++ ){
		tmp_data = ((DATA_T*)buffer)[i];
		
		if (tmp_data!=data[i]){
			return FALSE;
		}
	}

	free(buffer);

	return TRUE;
}
#endif
