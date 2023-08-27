#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[]){
    int n = 100, i, count = 0; // Default
    double x, y, pi, Error;

    for (i=0; i<argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            i++;
            n = atoi(argv[i]);
        } 
    }

    srand(time(0));


    for (i = 0; i < n; i++) {
        x = (double)rand() / RAND_MAX; 
        y = (double)rand() / RAND_MAX; 

        if (sqrt(x * x + y * y) <= 1) {
            count++;
        }
    }

    pi = 4.0 * count / n;

    printf("Estimated value of Pi: %f\n", pi);

    Error = fabs(pi - M_PI) / M_PI * 100;
    printf("Percentage error: %f%%\n", Error);

    return 0;
}
