#!/bin/bash

# Remove existing log file if any
rm -f mpi_mat_timings_we.log
mpicc -o mpi_mat_multiply_we mpi_mat_multiply_we.c -lm -std=c99

# Loop from 1 to 8 to run the MPI code with different numbers of nodes
for i in {1..8}
do
    echo "Running with $i nodes" | tee -a mpi_mat_timings_we.log
    stdbuf -oL mpirun -np $i ./mpi_mat_multiply_we | tee -a mpi_mat_timings_we.log
    echo "---------------------------" | tee -a mpi_mat_timings_we.log
done
