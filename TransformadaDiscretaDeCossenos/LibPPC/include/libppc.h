#ifndef __LIBPPC_H__

#define __LIBPPC_H__


#include <complex.h>

/**
 *  * \brief This macro is intended to help on matrixes algorithms
 * */ 
#define M(i, j, number_of_columns , mat) mat[ i*number_of_columns + j ]

#define BOOLEAN int
#define TRUE 1
#define FALSE 0

/**
	\brief A point in a 2-D space
*/
typedef struct {

	double x;
	double y;
	
} point2D_t;


/**
	\brief Print double vector pointed by data

	\param @data pointer to the data
	\param @size size of the vector
	\param @line_break breaks line after printing the specified number of here 
*/ 
void print_double_vector(const double *data, long int size, long int line_break);


/**
	\brief Print a double complex vector pointed by data

	\param @data pointer to the data
	\param @size size of the vector
	\param @line_break breaks line after printing the specified number of here 
*/ 
void print_double_complex_vector(const double complex *data, long int size, long int line_break);



/**
	\brief Print an integer vector pointed by data

	\param @data pointer to the data
	\param @size size of the vector
	\param @line_break breaks line after printing the specified number of here 
*/ 
void print_int_vector(const int *data, long int size, long int line_break);


/**
	\brief Saves a double vector pointed by data on a specified filename

	The data is saved as a double type - not as chars

	\param @data pointer to the data
	\param @size size of the vector
	\param @filename name of the file to save the vector

	\return 0 on success
*/ 
int save_double_vector(const double *data, long int size, const char *filename );


/**
	\brief Saves a double complex vector pointed by data on a specified filename

	The data is saved as a double type - not as chars

	\param @data pointer to the data
	\param @size size of the vector
	\param @filename name of the file to save the vector

	\return 0 on success
*/ 
int save_double_complex_vector(const double complex *data, long int size, const char *filename );



/**
	\brief Saves an integer vector pointed by data on a specified filename

	The data is saved as an integer type - not as chars

	\param @data pointer to the data
	\param @size size of the vector
	\param @filename name of the file to save the vector

	\return 0 on success
*/ 
int save_int_vector(const int *data, long int size, const char *filename );


/**
	\brief Loads a file containing a double vector
	

	Example: 
	double * vector = load_double_vector( "file_with_doubles.dat", N );

	\param filename name of the file to load the vector
	\param size size of the vector

	\return a pointer on success, NULL pointer on failure
*/ 
double* load_double_vector(const char *filename, long int size);

/**
	\brief Loads a file containing an integer vector
	
	Example: 
	double * vector = load_double_vector( "file_with_doubles.dat", N );

	\param filename name of the file to load the vector
	\param size size of the vector

	\return a pointer on success, NULL pointer on failure
*/ 
int* load_int_vector(const char *filename, long int size);




/**
	\brief Fills a double vetor with random numbers

	\param pointer_to_data Pointer to the vector with data
	\param quantity the quantity of data to generate
	\param minvalue the lowest number to generate
	\param maxvalue the highest value to generate
*/
double* generate_random_double_vector(long int quantity, double minvalue, double maxvalue);


/**
	\brief Fills a double vetor with random numbers

	\param pointer_to_data Pointer to the vector with data
	\param quantity the quantity of data to generate
	\param minvalue the lowest number that can be generated
	\param maxvalue the highest number that can be generated
*/
int* generate_random_int_vector(long int quantity, int minvalue, int maxvalue);


/**
	\brief Generates a point vector on a 2-D space

	\param quantity the quantity of data to generate
	\param minvalue the lowest number that can be generated
	\param maxvalue the highest number that can be generated
*/
point2D_t* generate_random_2Dpoints_vector(long int quantity, double minvalue, double maxvalue);


/**
	\brief Compares 2 vectors stored on main memory

	\return 0 if the vectors are the same, 1 otherwise
*/
int compare_double_vectors(const double *vector1, const double *vector2, long int size);



/**
 * \brief Compares two vectors stored on files
 * 
 * \return 1 if the vectors are the same, 0 otherwise
*/
int compare_int_vectors_on_files(const char *vector_file1, const char *vector_file2);



/**
 * \brief Compares two vectors stored on files
 * 
 * \return 1 if the vectors are the same, 0 otherwise
*/
int compare_double_vector_on_files(const char *vector_file1, const char *vector_file2);


/**
 * \brief Compares two double vectors stored on files
 * 
 * \return 1 if the vectors are the same, 0 otherwise
*/
int compare_double_complex_vector_on_files(const char *vector_file1, const char *vector_file2);


/**
	\brief Print a doubles matrix

*/ 
void print_double_matrix(double *matrix, 
	long int lines, 
	long int columns);

/**
 * \brief Generates a random double matrix
 * 
 * The numbers generated will be between the number of elements (lines x columns).
 * 
 * The programmer MUST free the allocated memory after its use!
 * 
*/
double* generate_random_double_matrix( 
	long int lines, 
	long int columns);

/**
 * \brief Saves a double matrix pointed by data on a specified filename
 * 
 * The data is saved as a double type - not as chars
 * 
 * \param matrix pointer to the matrix
 * \param size size of the vector
 * \param filename name of the file to save the vector
 * 
 * \return 0 on success
 * */
int save_double_matrix(const double *matrix, 
	long int number_of_lines, 
	long int number_of_columns,
	const char *filename );


/**
 * \brief Loads a matrix saved on a file
 * 
 * \param filename Name of the file with the saved matrix
 * \param number_of_lines Number of lines of the matrix to load
 * \param number_of_columns Number of columns of the matrix to load
 * 
 * \return A pointer on success, NULL on an error 
*/ 
double* load_double_matrix(const char *filename,
	long int number_of_lines,
	long int number_of_columns
	);

/**
	\brief Compares 2 matrixes stored on main memory

	\return 1 if the matrixes are the same, 0 otherwise
*/
int compare_double_matrixes(const double **matrix1, 
	const double **matrix2, 
	long int lines,
	long int columns);

/**
	\brief Compares 2 matrixes stored on files

	\return 1 if the matrixes are the same, 0 if not
*/
int compare_double_matrixes_on_files(const char *filename1, 
	const char *filename2,
	long int number_of_lines,
	long int number_of_columns);


#if 0
/*
	\brief save current matrix on the file filename

		the file must be the same saved by the function save_matrix

	\param @data pointer to the data
	\param @size number of lines of the matrix
	\param @filename file to save the matrix
*/
int compare_double_vectors(DATA_T *data, size_t size, const char *filename);

/*
	\brief print matrix pointed by data

	\param data pointer to the data
	\lines number of lines of the matrix
	\cols number of columns of the matrix	
*/ 
void print_matrix(DATA_T *data, size_t lines, size_t cols, long int line_break);

/*
	\brief save current matrix on the file filename

	\param @data pointer to the data
	\param @lines number of lines of the matrix
	\param @cols number of columns of the matrix	
	\param @filename file to save the matrix
*/
void save_matrix(DATA_T *data, size_t lines, size_t cols, const char *filename);

/*
	\brief save current matrix on the file filename

		the file must be the same saved by the function save_matrix

	\param @data pointer to the data
	\param @lines number of lines of the matrix
	\param @cols number of columns of the matrix	
	\param @filename file to save the matrix
*/
int compare_matrix(DATA_T *data, size_t lines, size_t cols, const char *filename);


// checks if 2 matrixes are equal
//int check_matrix(DATA_T *data1, DATA_T *data2, size_t lines, size_t cols);

#endif

#endif
