# Matrix Inverter

Inverts a user-specified square (NxN), non-singular matrix using Gaussian elimination and backsubstitution. Outputs the inverted matrix to 3 decimal places.

Warning: this program implements partial, not complete, pivoting, so rounding errors may occur for some matrices.

## How to format the input file

1. The file should be named 'matrix.txt' and placed in the working directory.
2. There should be no blank lines at the beginning or end of the file.
3. Each row should start at the beginning of a new line.
4. There should be no spaces at the beginning or end of each row.
5. The matrix elements on each row should be separated by a comma.
6. There should be no commas at the beginning or end of each row.

Example:
```
0,1,0,2
1,0,1,0
0,1,0,1
1,0,2,0
```

Note: you can choose to generate an example file in the working directory named 'matrix.txt'.
