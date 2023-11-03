#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void mat_vec_multiply(double *A, double *B, double *result, int size, double d) {
    // if we want to consider the damping factor, we should add this line: double one_minus_d_over_N = (1.0 - d) / (double)size;
    for (int i = 0; i < size; ++i) {
        result[i] = 0.0;
        for (int j = 0; j < size; ++j) {
            result[i] += A[i * size + j] * B[j];  // The damping parameter should be added here if needed
        }
        // if we want to consider the damping factor, we should add this line: result[i] += one_minus_d_over_N;
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 4000;
    const int num_iterations = 100;
    const double d = 0.85;
    double *A = malloc(N * N * sizeof(double));
    double *B = malloc(N * sizeof(double));
    double *result = malloc(N * sizeof(double));

    // Initialize A and B
	if (rank == 0) {
        for (i = 0; i < N; ++i) {
            B[i] = rand() / (double)RAND_MAX;
            for (j = 0; j < N; ++j) {
                A[i * N + j] = rand() / (double)RAND_MAX;
            }
        }
    }

    double start_time, end_time;
    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    for (int i = 0; i < num_iterations; ++i) {
        MPI_Bcast(B, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        mat_vec_multiply(A, B, result, N, d);
        if (rank == 0) {
            for (int j = 0; j < N; ++j) {
                B[j] = result[j];
            }
        }
    }

    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Time taken with %d MPI tasks: %f seconds\n", size, end_time - start_time);
    }

    free(A);
    free(B);
    free(result);

    MPI_Finalize();
    return 0;
}