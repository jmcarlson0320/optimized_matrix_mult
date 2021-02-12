#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float **build_matrix(int n)
{
    float **mat = malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(float));
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = (float) rand() / (float) RAND_MAX;
        }
    }

    return mat;
}


void free_matrix(float **mat, int n)
{
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }

    free(mat);
}


void print_matrix(float **mat, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f\t", mat[i][j]);
        }
        printf("\n");
    }
}


void serial_mult(float **a, float **b, int n)
{
}


int main(int argc, char *argv[])
{
    printf("matrix multiplier\n");
    float **matrix = NULL;
    matrix = build_matrix(3);
    print_matrix(matrix, 3);
    free_matrix(matrix, 3);
    return 0;
}
