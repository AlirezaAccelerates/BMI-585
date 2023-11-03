#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <immintrin.h>  // For AVX intrinsics
#include <stdint.h>

// State for the xorshift128+ random number generator.
static uint64_t s[2];

// Implementation of the xorshift128+ random number generator.
uint64_t xorshift128plus(void) {
    uint64_t x = s[0];
    uint64_t const y = s[1];
    s[0] = y;
    x ^= x << 23; // a
    s[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
    return s[1] + y;
}

// Function to seed the xorshift128+ random number generator.
void seed_xorshift128plus(uint64_t seed1, uint64_t seed2) {
    s[0] = seed1;
    s[1] = seed2;
}

// Function to multiply matrices A and B and store the result in matrix C. It uses AVX instructions for optimization.
void matrix_multiply(float **A, float **B, float **C, int n) {
    int i, j, k;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j += 8) {
            __m256 result = _mm256_setzero_ps();

            for (k = 0; k < n; k++) {
                __m256 a_val = _mm256_broadcast_ss(&A[i][k]);

                // Handle the case where the last group may not have 8 elements.
                float b_values[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                for (int idx = 0; idx < 8 && j + idx < n; ++idx) {
                    b_values[idx] = B[k][j + idx];
                }
                __m256 b_vec = _mm256_loadu_ps(b_values);

                result = _mm256_add_ps(result, _mm256_mul_ps(a_val, b_vec));
            }

            float temp[8];
            _mm256_storeu_ps(temp, result);

            for (int idx = 0; idx < 8 && j + idx < n; ++idx) {
                C[i][j + idx] = temp[idx];
            }
        }
    }
}

// Main function which gives user options to specify matrix dimensions, display matrices and benchmark matrix multiplication.
int main(int argc, char *argv[]){
    int n = 3, display = 0, benchmark = 0 ,i ,j, k; // Default values.

    // Parse command line arguments.
    for (i=0; i<argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            i++;
            n = atoi(argv[i]);
        } else if (strcmp(argv[i], "-display") == 0){
            display = 1;
        } else if (strcmp(argv[i], "-benchmark") == 0){
            benchmark = 1;
        }
    }

    // Seed the xorshift128+ random number generator.
    seed_xorshift128plus(time(0), time(0) ^ 0xDEADBEEFCAFEBABE);

    // Allocate memory for matrices A and C.
    float **A = malloc(n * sizeof(float *));
    float **C = malloc(n * sizeof(float *));
    for (i = 0; i < n; ++i) {
        A[i] = malloc(n * sizeof(float));
        C[i] = malloc(n * sizeof(float));
    }

    // Fill matrix A with random float numbers between 0 and 3.
    for (i=0; i<n; ++i) {
        for (j=0; j<n; ++j) {
            A[i][j] = ((float)xorshift128plus() / UINT64_MAX) * 3;
        }
    }

    // Perform matrix multiplication: C = A x A.
    matrix_multiply(A, A, C, n);

    // If the user has requested to display matrices.
    if(display) {
        printf("Matrix A:\n");
        for (i=0; i<n; ++i) {
            for (j = 0; j < n; ++j) {
                printf("%.2f ", A[i][j]);
            }
            printf("\n");
        }
        printf("\nResult of A x A:\n");
        for(i=0; i<n; i++) {
            for(j=0; j<n; j++) {
                printf("%.2f ", C[i][j]);
            }
            printf("\n");
        }
    }

    // Free the allocated memory for matrices A and C.
    for (i = 0; i < n; i++) {
        free(A[i]);
        free(C[i]);
    }
    free(A);
    free(C);

    // Benchmarking matrix multiplication for varying matrix sizes.
    if (benchmark) {
        double total_time = 0;
        for(n=2; n<=4096; n *= 2) {
            A = malloc(n * sizeof(float *));
            C = malloc(n * sizeof(float *));
            for (i = 0; i < n; ++i) {
                A[i] = malloc(n * sizeof(float));
                C[i] = malloc(n * sizeof(float));
            }
            for (i=0; i<n; ++i) {
                for (j=0; j<n; ++j) {
                    A[i][j] = (float)rand() / RAND_MAX * 3; 
                }
            }
            
            // Measure time taken for matrix multiplication.
            clock_t start, end;
            double cpu_time_used;

            start = clock();
            matrix_multiply(A, A, C, n);
            end = clock();

            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

            printf("Time taken for %dx%d matrix: %f seconds\n", n, n, cpu_time_used);

            total_time += cpu_time_used;

            // Free memory after each iteration.
            for(i = 0; i < n; i++) {
                free(A[i]);
                free(C[i]);
            }
            free(A);
            free(C);
        }
        printf("Total time taken %f seconds\n", total_time);
    }

    return 0;
}