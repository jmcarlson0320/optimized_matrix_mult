#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 3

float **random_matrix(int n)
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


float **empty_matrix(int n)
{
    float **mat = malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(float));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = 0;
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


void print_matrix(float **mat, int n, char *name)
{
    printf("%s\n", name);
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
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = 0.0f;
            for (int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}


typedef struct thread_data thread_data;
struct thread_data {
    int thread_num;
    float **A;
    float **B;
    float **C;
    int matrix_size;
};


void *parallel_mult(void *arg)
{
    int id = *(int *) arg;
    printf("thread %d running parallel_mult()\n", id);
    return NULL;
}


int main(int argc, char *argv[])
{
    printf("matrix multiplier\n");
    int size = 3;
    if (argc == 2)
        size = atoi(argv[1]);

    float **m_1 = NULL;
    m_1 = random_matrix(size);
    print_matrix(m_1, size, "A");

    float **m_2 = NULL;
    m_2 = ident_matrix(size);
    print_matrix(m_2, size, "B");

    float **m_3 = NULL;
    m_3 = empty_matrix(size);

    pthread_t threads[NUM_THREADS];
    int thread_num[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_num[i] = i;
        pthread_create(&threads[i], NULL, parallel_mult, &thread_num[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }


    serial_mult(m_3, m_1, m_2, size);

    print_matrix(m_3, size, "C = AxB");

    return 0;
}
