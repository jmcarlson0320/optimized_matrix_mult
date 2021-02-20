# CS431 Homework 5

This program performs matrix multiplication. It generates a random n x n
matrix and multiplies it by the identity matrix.

## Installation

Specify optimization level by setting the OPT_LVL flag in the Makefile.

    - OPT_0 = none
    - OPT_1 = accumulate sums locally
    - OPT_2 = accumulate sums locally AND transpose the second matrix

Build the program.

```bash
make
```

## Usage

```bash
./app [-t <number of threads>] [-n <matrix size>] [-p]
```
The -p flag prints the matrices to the console.
