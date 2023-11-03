#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define REAL_PI acos(-1)

int main(int argc, char *argv[]) {
    int p, i, count; 
    double x, y, pi, Error;
    FILE *output;

    // Open output file for saving results
    output = fopen("./pi_results.csv", "w");
    if (output == NULL) {
        fprintf(stderr, "Could not open output file.\n");
        return 1;
    }

    // Write header to the CSV file
    fprintf(output, "p,n,pi,Error,runtime\n");

    for (p = 1; p <= 8; p++) {
        // Set the number of threads for OpenMP
        omp_set_num_threads(p);

        for (int n = 20; n <= 200000000; n *= 10) {
            // Reset count
            count = 0;

            // Capture the start time
            clock_t start_time = clock();

            unsigned int seed;

            #pragma omp parallel for private(x, y, seed) reduction(+:count)
            for (i = 0; i < n; i++) {
                seed = (unsigned int)time(0) ^ omp_get_thread_num(); // Unique seed for each thread
                x = (double)rand() / RAND_MAX;
                y = (double)rand() / RAND_MAX;

                if (sqrt(x * x + y * y) <= 1) {
                    count++;
                }
            }

            pi = 4.0 * count / n;

            // Calculate the error
            Error = fabs(pi - REAL_PI) / REAL_PI * 100;

            // Calculate and print the runtime
            clock_t end_time = clock();
            double runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;

            // Save results to the text file
            fprintf(output, "%d,%d,%f,%f,%f\n", p, n, pi, Error, runtime);
        }
    }

    // Close output file
    fclose(output);

    return 0;
}
