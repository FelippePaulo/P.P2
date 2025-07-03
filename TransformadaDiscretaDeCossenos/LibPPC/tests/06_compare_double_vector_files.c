#include <libppc.h>

#include <stdlib.h>

#include <stdio.h>

int main(){

    /**
     * Test 1: 512 B aligned equal vectors 
     * */ 

    double *v1 = (double*)malloc( sizeof(double)*1024 );
    double *v2 = (double*)malloc( sizeof(double)*1024 );

    for ( long unsigned i=0; i < 1024; i++){

        v1[ i ] = v2[ i ] = (double)i;

    }

    int ret1 = save_double_vector(v1,1024,"file1.test1.input");
    // Error saving file - abort 
    if ( ret1 != 0 ){
        return -11;
    } 

    int ret2 = save_double_vector(v2,1024,"file2.test1.input");
    // Error saving file - abort 
    if ( ret1 != 0 ){
        return -12;
    }

    int result = compare_double_vector_on_files( "file1.test1.input", "file2.test1.input" );
    // Both files are equal, should return 1
    if ( result != 1 ){
        return 1;   
    }

    /**
     * Test 2: Vector 1 has lower number of elements
     * */ 

    ret1 = save_double_vector(v1,1023,"file1.test2.input");
    // Error saving file - abort 
    if ( ret1 != 0 ){
        return -21;
    } 

    ret2 = save_double_vector(v2,1024,"file2.test2.input");
    // Error saving file - abort 
    if ( ret1 != 0 ){
        return -22;
    }

    result = compare_double_vector_on_files( "file1.test2.input", "file2.test2.input" );
    // Files have different sizes, should return 0
    if ( result != 0 ){
        return 2;    
    }

    /**
     * Test 3: Vector 2 has lower number of elements
    */
    ret1 = save_double_vector( v1, 1024, "file1.test3.input");
    // Error saving file - abort 
    if ( ret1 != 0 ){
        return -31;
    } 

    ret2 = save_double_vector( v1, 1023, "file2.test3.input");
    // Error saving file - abort 
    if ( ret1 != 0 ){
        return -32;
    } 

    result = compare_double_vector_on_files( "file1.test3.input", "file2.test3.input" );
    // Files have different sizes, should return 0
    if ( result != 0 ){
        return 3;    
    }

    return 0;
}

