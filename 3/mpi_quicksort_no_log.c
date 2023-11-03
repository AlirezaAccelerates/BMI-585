#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int partition(double *arr, int low, int high) {
    double pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            double t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }
    double t = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = t;
    return (i + 1);
}

void quicksort(double *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time, end_time;

    int n = 1000000;
    double *arr = NULL;
    double *local_arr = (double*) malloc(n / size * sizeof(double));

    if (rank == 0) {
        arr = (double*) malloc(n * sizeof(double));
        for (int i = 0; i < n; i++) {
            arr[i] = ((double) rand() / (double) RAND_MAX);
        }
    }

    start_time = MPI_Wtime();

    MPI_Scatter(arr, n / size, MPI_DOUBLE, local_arr, n / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    quicksort(local_arr, 0, n / size - 1);

    MPI_Gather(local_arr, n / size, MPI_DOUBLE, arr, n / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        quicksort(arr, 0, n - 1);
    }

    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Nodes: %d, Time: %f\n", size, end_time - start_time);  
        free(arr);
    }

    free(local_arr);

    MPI_Finalize();
    return 0;
}