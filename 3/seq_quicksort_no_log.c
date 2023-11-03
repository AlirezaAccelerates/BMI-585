#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

int main() {
    int n = 1000000;
    double *arr = (double*) malloc(n * sizeof(double));

    // Seed the random number generator for reproducibility
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        arr[i] = ((double) rand() / (double) RAND_MAX);
    }

    // Record start time
    clock_t start_time = clock();

    // Perform quicksort on the entire array
    quicksort(arr, 0, n - 1);

    // Record end time and calculate duration
    clock_t end_time = clock();
    double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Time: %f\n", elapsed_time);
    
    // Free allocated memory
    free(arr);

    return 0;
}