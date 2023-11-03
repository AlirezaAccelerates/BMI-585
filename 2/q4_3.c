#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <immintrin.h>  // For AVX intrinsics
#include <stdint.h>
#include <omp.h>  // For OpenMP

static uint64_t s[2];
#pragma omp threadprivate(s)

uint64_t xorshift128plus(void) {
    uint64_t x = s[0];
    uint64_t const y = s[1];
    s[0] = y;
    x ^= x << 23; // a
    s[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
    return s[1] + y;
}

void seed_xorshift128plus(uint64_t seed1, uint64_t seed2) {
    s[0] = seed1;
    s[1] = seed2;
}

void matrix_multiply(float **A, float **B, float **C, int n) {
    int i, j, k;

    #pragma omp parallel for private(j, k)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j += 8) {
            __m256 result = _mm256_setzero_ps();

            for (k = 0; k < n; k++) {
                __m256 a_val = _mm256_broadcast_ss(&A[i][k]);
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

int main(int argc, char *argv[]) {
    int n = 3, display = 0, benchmark = 1 ,i ,j, k;
    FILE *fp = fopen("results.csv", "w");
    fprintf(fp, "p,n,runtime\n");

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            i++;
            n = atoi(argv[i]);
        } else if (strcmp(argv[i], "-display") == 0) {
            display = 1;
        } else if (strcmp(argv[i], "-benchmark") == 0) {
            benchmark = 1;
        }
    }

    for (int p = 1; p <= 8; p++) {
        omp_set_num_threads(p);

        for (n = 2; n <= 4096; n *= 2) {
            #pragma omp parallel
            {
                uint64_t seed = time(0) + omp_get_thread_num();
                seed_xorshift128plus(seed, seed ^ 0xDEADBEEFCAFEBABE);
            }

            float **A = malloc(n * sizeof(float *));
            float **C = malloc(n * sizeof(float *));
            for (i = 0; i < n; ++i) {
                A[i] = malloc(n * sizeof(float));
                C[i] = malloc(n * sizeof(float));
            }

            for (i = 0; i < n; ++i) {
                for (j = 0; j < n; ++j) {
                    A[i][j] = ((float)xorshift128plus() / UINT64_MAX) * 3;
                }
            }

            clock_t start, end;
            start = clock();
            matrix_multiply(A, A, C, n);
            end = clock();

            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            fprintf(fp, "%d,%d,%f\n", p, n, cpu_time_used);

            if (display) {
                printf("Matrix A:\n");
                for (i = 0; i < n; ++i) {
                    for (j = 0; j < n; ++j) {
                        printf("%.2f ", A[i][j]);
            }
            printf("\n");
        }
        printf("\nResult of A x A:\n");
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                printf("%.2f ", C[i][j]);
            }
            printf("\n");
        }
            }

            for (i = 0; i < n; i++) {
                free(A[i]);
                free(C[i]);
            }
            free(A);
            free(C);
        }
    }

    fclose(fp);
    return 0;
}