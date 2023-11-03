#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <immintrin.h> // For AVX2 intrinsics
#include <string.h>
#include <stdint.h>    // For uint64_t data type
#include <sys/time.h>  // For getting detailed timing information

// Structure representing the state for the xorshift128+ random number generator
typedef struct {
    uint64_t s[2];
} xorshift128plus_state;

// Seeds the xorshift128+ state with a provided seed
void seed_xorshift128plus(xorshift128plus_state* state, uint64_t seed) {
    state->s[0] = seed;
    state->s[1] = seed ^ 0xA564DE645ED39A05ULL;
}

// Generates a random number using xorshift128+ algorithm
uint64_t xorshift128plus(xorshift128plus_state* state) {
    uint64_t s1 = state->s[0];
    const uint64_t s0 = state->s[1];
    state->s[0] = s0;
    s1 ^= s1 << 23; 
    state->s[1] = s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26);
    return state->s[1] + s0;
}

int main(int argc, char *argv[]){
    int n = 100, i, count = 0;  // Default number of samples, loop counters, and points inside the unit circle
    double pi, Error;  // Variables to hold estimated value of Pi and its percentage error
    xorshift128plus_state rng_state;  // State for the random number generator

    // Parse command line arguments
    for (i = 0; i < argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            i++;
            n = atoi(argv[i]);
        } 
    }

    // Open a file to store the error results over time
    FILE *f = fopen("error_results.txt", "w");
    if (!f) {
        perror("Failed to open error_results.txt");
        exit(1);
    }

    fprintf(f, "Time,Error\n");  // Write headers to the file

    // Seed the random number generator
    seed_xorshift128plus(&rng_state, time(0));

    // AVX2 intrinsic setup
    __m256 ones = _mm256_set1_ps(1.0f);  // Vector of ones for comparison

    struct timeval start_time, current_time, end_time;  // Structs to hold timing data
    gettimeofday(&start_time, NULL);  // Record the start time

    // Monte Carlo estimation of Pi
    for (i = 0; i < n; i+=8) {
        float x_vals[8], y_vals[8];  // Arrays to store x and y coordinates

        // Generate random coordinates using the xorshift128+ algorithm
        for (int j = 0; j < 8; j++) {
            x_vals[j] = (double)(xorshift128plus(&rng_state) & ((1ULL << 52) - 1)) / (1ULL << 52);
            y_vals[j] = (double)(xorshift128plus(&rng_state) & ((1ULL << 52) - 1)) / (1ULL << 52);
        }

        // Load the coordinates into AVX2 vectors
        __m256 x = _mm256_loadu_ps(x_vals);
        __m256 y = _mm256_loadu_ps(y_vals);
        __m256 len_squared = _mm256_add_ps(_mm256_mul_ps(x, x), _mm256_mul_ps(y, y));

        // Determine which points are inside the unit circle
        __m256 mask = _mm256_cmp_ps(len_squared, ones, _CMP_LE_OS);

        // Count the number of points inside the circle
        int int_mask[8];
        _mm256_storeu_si256((__m256i*)int_mask, _mm256_castps_si256(mask));
        for (int j = 0; j < 8; j++) {
            count += (int_mask[j] != 0) ? 1 : 0;
        }

        // Periodically compute and log the error
        if (i % (n/100) == 0) { 
            double pi_estimate = 4.0 * count / i;
            double error = fabs(pi_estimate - acos(-1)) / acos(-1) * 100;

            gettimeofday(&current_time, NULL);
            long elapsed_microseconds = (current_time.tv_sec - start_time.tv_sec) * 1e6 + (current_time.tv_usec - start_time.tv_usec);

            fprintf(f, "%ld,%f\n", elapsed_microseconds, error);
        }
    }

    fclose(f);  // Close the file
    
    gettimeofday(&end_time, NULL);  // Record the end time

    // Compute the elapsed time
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long microseconds = end_time.tv_usec - start_time.tv_usec;
    double elapsed = seconds + microseconds*1e-6;

    printf("Time taken: %f seconds (%f microseconds)\n", elapsed, elapsed*1e6);

    // Compute and display the final estimated value of Pi and its error
    pi = 4.0 * count / n;
    printf("Estimated value of Pi: %f\n", pi);

    Error = fabs(pi - acos(-1)) / acos(-1) * 100;
    printf("Percentage error: %f%%\n", Error);

    return 0;
}