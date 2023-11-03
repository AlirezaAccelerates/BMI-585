        for (i = 0; i < local_rows; ++i) {
            for (j = 0; j < N; ++j) {
                A[i * N + j] = rand() / (double)RAND_MAX;
            }
            B[i] = rand() / (double)RAND_MAX;
        }