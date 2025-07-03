#include <libppc.h>

#include <stdlib.h>

#include <stdio.h>

// TODO: test negative numbers

int main(){

    double v1[] = { 1.0, 2.2, 3.3};
    
    double v2[] = { 1.0, 2.2, 3.3};

    int ret = compare_double_vectors( v1, v2, 3);

    if (ret != 0)
        return 1;

    v2[ 2 ] = -1.1;

    ret = compare_double_vectors( v1, v2, 3);

    if (ret == 0)
        return 2;

    return 0;
}

