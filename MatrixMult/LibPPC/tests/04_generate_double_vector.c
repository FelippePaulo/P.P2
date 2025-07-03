#include <libppc.h>

#include <stdlib.h>

#include <stdio.h>

// TODO: test negative numbers

int main(){

    double *v;

    v = generate_random_double_vector( 1000000, 0, 10);

    for (int i = 0; i < 1000000; i++){

        if ( v[ i ] >= 0 && v[i] <= 10 ){
         
            continue;

        }
            
        return -1;
    }

    free( v ) ;

    return 0;
}

