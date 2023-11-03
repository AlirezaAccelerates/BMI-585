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

**Estimated value of Pi: 3.152000**

**Percentage error: 0.331276%**

## Profile the scripts to determine what parts take up the most amount of time. Are their parts of your code that are impacted by the size of n? Any optimization strategies?
In my code, the "sqrt(x * x + y * y)" operation takes up the most amount of time and obviously, increasing the size of n has a considerable impact on the running time. 

The time complexity of the code is O(n), as it iterates n times to generate random points and do the math. This is directly influenced by the value of n, which means the larger n, the more time the code will take to run.

One optimization strategy could be avoiding "sqrt(x * x + y * y)" operation by using more creative and less time-consuming approaches.

