#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    double start_time, end_time;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int N = 4000;
    int local_rows = N / size; // Number of rows for each process
    double *A = NULL;
    double *local_A = malloc(local_rows * N * sizeof(double));
    double *B = malloc(N * sizeof(double));
    double *local_C = malloc(local_rows * sizeof(double));
    int i, j;

    if (rank == 0) {
        A = malloc(N * N * sizeof(double));
        // Initialize A and B only in root process
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j) {
                A[i * N + j] = rand() / (double)RAND_MAX;
            }
            B[i] = rand() / (double)RAND_MAX;
        }
        start_time = MPI_Wtime();
    }

    // Broadcast B to all processes
    MPI_Bcast(B, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter rows of A to all processes
    MPI_Scatter(A, local_rows * N, MPI_DOUBLE, local_A, local_rows * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Local computation of matrix-vector multiplication
    for (i = 0; i < local_rows; ++i) {
        local_C[i] = 0.0;
        for (j = 0; j < N; ++j) {
            local_C[i] += local_A[i * N + j] * B[j];
        }
    }
    
    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Weak scaling execution time with %d processes: %f seconds\n", size, end_time - start_time);
        free(A); // Free memory allocated for A in root process
    }

    // Free allocated memory
    free(local_A);
    free(B);
    free(local_C);
    
    MPI_Finalize();
    return 0;
}