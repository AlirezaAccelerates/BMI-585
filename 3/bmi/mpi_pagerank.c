#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void mat_vec_multiply(double *A, double *B, double *result, int size, int local_size, int offset, double d) {
    // if we want to consider the damping factor, we should add this line: double one_minus_d_over_N = (1.0 - d) / (double)size;
    for (int i = 0; i < local_size; ++i) {
        result[i] = 0.0;
        for (int j = 0; j < size; ++j) {
            result[i] += A[i * size + j] * B[j];
        }
        // if we want to consider the damping factor, we should add this line: result[i] += one_minus_d_over_N;
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int N = 4000;
    double *A, *local_A, *B, *result, *local_result;
    double start_time, end_time;
    const double d = 0.85;
    
    // Initialize B and A only on rank 0
    if (rank == 0) {
        A = malloc(N * N * sizeof(double));
        B = malloc(N * sizeof(double));
        result = malloc(N * sizeof(double));
        for (int i = 0; i < N; ++i) {
            B[i] = rand() / (double)RAND_MAX;
            for (int j = 0; j < N; ++j) {
                A[i * N + j] = rand() / (double)RAND_MAX;
            }
        }
    } else {
        B = malloc(N * sizeof(double));
    }
    
    int local_size = N / size;
    local_A = malloc(local_size * N * sizeof(double));
    local_result = malloc(local_size * sizeof(double));
    
    MPI_Bcast(B, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, local_size * N, MPI_DOUBLE, local_A, local_size * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if (rank == 0) start_time = MPI_Wtime();
    
    mat_vec_multiply(local_A, B, local_result, N, local_size, rank * local_size, d);
    
    MPI_Gather(local_result, local_size, MPI_DOUBLE, result, local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Execution time with %d processes: %f seconds\n", size, end_time - start_time);
        free(A);
        free(result);
    }
    
    free(local_A);
    free(B);
    free(local_result);
    
    MPI_Finalize();
    return 0;
}
