#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<omp.h>

void matrix_multiply(float **A, float **C, int n){
    int i, j, k;
    #pragma omp parallel for private(i, j, k)
    for (i=0;i<n;i++){
        for (j=0;j<n;j++){
            C[i][j] = 0;
            for (k=0;k<n;k++){
                C[i][j] += A[i][k] * A[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]){
    FILE *fp = fopen("matrix_results.csv", "w"); // Open a CSV file for writing
    fprintf(fp, "p,n,Time\n"); // Write header

    int num_procs, n, i, j;
    for (num_procs = 1; num_procs <= 8; ++num_procs) { // Loop through 1 to 8 processors
        omp_set_num_threads(num_procs);

        for (n = 2; n <= 4096; n *= 2) { // Loop through matrix sizes from 2 to 4098 with a stride of *2

            float **A = malloc(n * sizeof(float *));
            float **C = malloc(n * sizeof(float *));
            for (i = 0; i < n; ++i) {
                A[i] = malloc(n * sizeof(float));
                C[i] = malloc(n * sizeof(float));
            }

            for (i = 0; i < n; ++i) {
                for (j = 0; j < n; ++j) {
                    A[i][j] = (float)rand() / RAND_MAX * 3;
                }
            }

            clock_t start, end;
            double cpu_time_used;

            start = clock();
            matrix_multiply(A, C, n);
            end = clock();

            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

            fprintf(fp, "%d,%d,%f\n", num_procs, n, cpu_time_used); // Write to CSV

            for (i = 0; i < n; ++i) {
                free(A[i]);
                free(C[i]);
            }
            free(A);
            free(C);
        }
    }

    fclose(fp); // Close the CSV file
    return 0;
}