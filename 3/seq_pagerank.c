#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mat_vec_multiply(double *A, double *B, double *result, int size, double d) {
    double one_minus_d_over_N = (1.0 - d) / (double)size;
    for (int i = 0; i < size; ++i) {
        result[i] = 0.0;
        for (int j = 0; j < size; ++j) {
            result[i] += d * A[i * size + j] * B[j];
        }
        result[i] += one_minus_d_over_N;
    }
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    const int N = 4000;
    const int num_iterations = 100;
    const double d = 0.85;
    double *A = malloc(N * N * sizeof(double));
    double *B = malloc(N * sizeof(double));
    double *result = malloc(N * sizeof(double));

    // Initialize A and B 
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = (double) rand() / RAND_MAX;
        }
        B[i] = (double) rand() / RAND_MAX;
    }

    // Record start time
    clock_t start_time = clock();

    for (int i = 0; i < num_iterations; ++i) {
        mat_vec_multiply(A, B, result, N, d);
        for (int j = 0; j < N; ++j) {
            B[j] = result[j];
        }
    }

    // Record end time and calculate duration
    clock_t end_time = clock();
    double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", elapsed_time);

    free(A);
    free(B);
    free(result);

    return 0;
}
