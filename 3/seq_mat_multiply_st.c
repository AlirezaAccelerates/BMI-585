#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int N = 4000;
    int local_rows = 1000;
    double *A = malloc(local_rows * N * sizeof(double));
    double *B = malloc(N * sizeof(double));
    double *local_C = malloc(local_rows * sizeof(double));
    int i, j;
    
    // Initialize A and B
    for (i = 0; i < local_rows; ++i) {
        for (j = 0; j < N; ++j) {
            A[i * N + j] = rand() / (double)RAND_MAX;
        }
        B[i] = rand() / (double)RAND_MAX;
    }
    
    // Timing starts here
    clock_t start_time = clock();
    
    // Local computation of matrix-vector multiplication
    for (i = 0; i < local_rows; ++i) {
        local_C[i] = 0.0;
        for (j = 0; j < N; ++j) {
                local_C[i] += A[i * N + j] * B[j];
        }
    }
    printf("Execution time: %d ", local_C);
    // Timing ends here
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", execution_time);
    
    // Free allocated memory
    free(A);
    free(B);
    free(local_C);

    return 0;
}