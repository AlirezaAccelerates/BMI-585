#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<time.h>

// Matrices Multiplication
void matrix_multiply(float **A, float **C, int n){
    int i, j, k;
    for (i=0;i<n;i++){
        for (j=0;j<n;j++){
            C[i][j] = 0;
            for (k=0;k<n;k++){
                C[i][j] += A[i][k] * A[k][j];
            }
        }
    }
}


// Give user the options on matrix dimention, display and benchmark
int main(int argc, char *argv[]){
    int n = 3, display = 0, benchmark = 0 ,i ,j, k; // Default

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

    srand(time(0));

    float **A = malloc(n * sizeof(float *));
    float **C = malloc(n * sizeof(float *));
    for (i = 0; i < n; ++i) {
        A[i] = malloc(n * sizeof(float));
        C[i] = malloc(n * sizeof(float));
    }

    for (i=0; i<n; ++i) {
        for (j=0; j<n; ++j) {
            A[i][j] = (float)rand() / RAND_MAX * 3; // A random float number between 0 to 3
        }
    }

    matrix_multiply(A, C, n);

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

    for (i = 0; i < n; i++) {
        free(A[i]);
        free(C[i]);
    }
    free(A);
    free(C);


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
            clock_t start, end;
            double cpu_time_used;

            start = clock();
            matrix_multiply(A, C, n);
            end = clock();

            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

            printf("Time taken for %dx%d matrix: %f seconds\n", n, n, cpu_time_used);

            total_time += cpu_time_used;

            for(i = 0; i < n; i++) {
                free(A[i]);
                free(C[i]);
            }
            free(A);
            free(C);
        }
        printf("Total time taken %f", total_time);
    }

    return 0;
}