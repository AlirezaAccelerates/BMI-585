# Alireza Rafiei | HW 0 | Q2

## Matrix Multiplication
Calculate the value of π

•	Throw n random points on a square

•	Count those points that land in a circle

•	PI = 4 * num of points in the circle / num of points


The files for this assignments are:

`Q2.c` is the source code file.

`Q2.exe` is the executable file.

`Q2.sh` is the shell script file.

## How do I run the script?

    .\Q2 -n 500


## Experimental results:

Estimated value of Pi: 3.152000

Percentage error: 0.331276%

## Profile the scripts to determine what parts take up the most amount of time. Are their parts of your code that are impacted by the size of n? Any optimization strategies?
In my code, the "matrix_multiply" matrix takes up the most amount of time and obviously, increasing the size of n has a considerable impact on the running time. 

The time complexity of the matrix_multiply function is determined by its nested loops. Thus, the time complexity of the function is cubic, O(n^3).

One optimization strategy that came into my mind is dividing the matrices into smaller blocks and then multiplying these blocks and maybe applying parallelization techniques.

