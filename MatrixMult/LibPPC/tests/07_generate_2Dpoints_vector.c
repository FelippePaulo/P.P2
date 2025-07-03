#include <libppc.h>

#include <stdlib.h>

#include <stdio.h>

// TODO: test negative numbers

int main(){

	point2D_t *v;


    v = generate_random_2Dpoints_vector( 1000000, 0, 10);

    for (int i = 0; i < 1000000; i++){
        	    	
        if ( v[ i ].x >= 0 && v[ i ].x <= 10 && v[ i ].y >= 0 && v[ i ].y <= 10  ){

            continue;

        }
            
        return -1;
    }

    free( v ) ;

    return 0;
}

