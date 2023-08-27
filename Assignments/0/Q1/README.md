# Alireza Rafiei | HW 0 | Q1

## Matrix Multiplication
•	Create a random n x n matrix  A

•	Multiply A x A

•	Give user the ability to specify the size of the matrix as an input parameter (-n 3)

•	Give user the option to display the input and product (-display)

•	Display how long it takes to run this on a
    2x2, 4x4, 8x8, 16x16…4096x4096 (-benchmark)

The files for this assignments are:

`Q1.c` is the source code file.

`Q1.exe` is the executable file.

`Q1.sh` is the shell script file.

## How do I run the script?

    .\Q1 -n 5 -display -benchmark

The functionality of each part of the previous command is explained in the question.

## Experimental results:

Matrix A:

1.69 2.30 2.77 1.76 1.09 

2.30 2.36 0.66 1.06 1.06

1.68 1.93 2.60 1.58 2.35

0.37 2.66 1.26 0.83 0.03

1.27 0.95 2.25 0.09 0.74


Result of A x A:

14.82 20.38 18.09 11.34 11.63

12.16 16.00 13.39 8.57 7.38 

15.20 19.88 19.98 10.62 11.75

9.19 11.79 7.17 6.15 6.22

9.08 10.45 11.78 6.93 8.21


Time taken for 2x2 matrix: 0.000000 seconds

Time taken for 4x4 matrix: 0.000000 seconds

Time taken for 8x8 matrix: 0.000000 seconds

Time taken for 16x16 matrix: 0.000000 seconds

Time taken for 32x32 matrix: 0.001000 seconds

Time taken for 64x64 matrix: 0.003000 seconds

Time taken for 128x128 matrix: 0.012000 seconds

Time taken for 256x256 matrix: 0.165000 seconds

Time taken for 512x512 matrix: 0.984000 seconds

Time taken for 1024x1024 matrix: 12.153000 seconds

Time taken for 2048x2048 matrix: 139.657000 seconds

Time taken for 4096x4096 matrix: 1915.101000 seconds

Total time taken 2068.076000 seconds

**Profile the scripts to determine what parts take up the most amount of time. Are their parts of your code that are impacted by the size of n? Any optimization strategies?**
In my code, the "matrix_multiply" matrix takes up the most amount of time and obviously, increasing the size of n has a considerable impact on the running time. 

The time complexity of the matrix_multiply function is determined by its nested loops. Thus, the time complexity of the function is cubic, O(n^3).

One optimization strategy that came into my mind is dividing the matrices into smaller blocks and then multiplying these blocks and maybe applying parallelization techniques.

