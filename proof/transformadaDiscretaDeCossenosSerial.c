#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

void print_vector(const double *v, long int N) {
    for (long int i = 0; i < N; i++) {
        printf("%.4lf ", v[i]);
    }
    printf("\n");
}

void DCT1D_serial(const double *input, double *output, long int N) {
    double ck = 1.0; // Fator de normalização para k=0
    
    for (long int k = 0; k < N; k++) {
        if (k > 0) ck = sqrt(2.0/N); // Fator de normalização para k>0
        else ck = sqrt(1.0/N);        // Fator especial para k=0
        
        double sum = 0.0;
        for (long int n = 0; n < N; n++) {
            sum += input[n] * cos(PI * (n + 0.5) * k / N);
        }
        output[k] = ck * sum;
        
        printf("Saída após k=%ld: ", k);
        print_vector(output, N);
    }
}

int main() {
    double input[] = {8, 16, 24, 32, 40, 48, 56, 64}; // Exemplo de vetor de entrada
    long int N = sizeof(input) / sizeof(input[0]);
    double output[8] = {0}; // Corrigido para tamanho N=8

    printf("Vetor de entrada: ");
    print_vector(input, N);

    DCT1D_serial(input, output, N);

    printf("Vetor final de saída (DCT): ");
    print_vector(output, N);

    return 0;
}