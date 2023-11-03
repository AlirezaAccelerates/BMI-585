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
    double *A = malloc(N * N * sizeof(double));
    double B[N], C[N], local_C[N / size];
    int i, j;
    
    
    // Initialize A and B only in the root process
    if (rank == 0) {
        for (i = 0; i < N; ++i) {
            B[i] = rand() / (double)RAND_MAX;
            for (j = 0; j < N; ++j) {
                A[i * N + j] = rand() / (double)RAND_MAX;
            }
        }
        start_time = MPI_Wtime();
    }
    
    // Broadcast B to all processes
    MPI_Bcast(B, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // Scatter rows of A to all processes
    MPI_Scatter(A, N * N / size, MPI_DOUBLE, &A[rank * N * N / size], N * N / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // Local computation of matrix-vector multiplication
    for (i = 0; i < N / size; ++i) {
        local_C[i] = 0.0;
        for (j = 0; j < N; ++j) {
            local_C[i] += A[(rank * N / size + i) * N + j] * B[j];
        }
    }
    
    // Gather the results back to the root process
    MPI_Gather(local_C, N / size, MPI_DOUBLE, C, N / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // Timing ends here
    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Execution time with %d processes: %f seconds\n", size, end_time - start_time);
    }
	
	if (A) {
        free(A);
    }
    
    MPI_Finalize();
    return 0;
}