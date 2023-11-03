#!/bin/bash

# Remove existing log file if any
rm -f mpi_timings_no_log.log
mpicc -o mpi_quicksort_no_log mpi_quicksort_no_log.c -lm -std=c99

# Loop from 1 to 8 to run the MPI code with different numbers of nodes
for i in {1..8}
do
    echo "Running with $i nodes" | tee -a mpi_timings_no_log.log
    stdbuf -oL mpirun -np $i ./mpi_quicksort_no_log | tee -a mpi_timings_no_log.log
    echo "---------------------------" | tee -a mpi_timings_no_log.log
done
