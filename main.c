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


float **ident_matrix(int n)
{
    float **mat = malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(float));
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                mat[i][j] = 1.0f;
            else
                mat[i][j] = 0.0f;
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


// c = a * b
void serial_mult(float **c, float **a, float **b, int n)
{

}


int main(int argc, char *argv[])
{
    printf("matrix multiplier\n");
    int size = 3;
    if (argc == 2)
        size = atoi(argv[1]);
    float **matrix = NULL;
    matrix = ident_matrix(size);
    print_matrix(matrix, size);
    free_matrix(matrix, size);
    return 0;
}
