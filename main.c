#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define DEFAULT_SIZE 3
#define DEFAULT_THREADS 1

void usage(void)
{
    printf("usage: ./app [-n <size>] [-t <number of threads>]\n");
}


float **allocate_matrix(int n)
{
    float **mat = malloc(n * sizeof(float *));
    if (!mat) {
        printf("error allocating matrix of size %d\n", n);
        exit(0);
    }

    for (int i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(float));
        if (!mat[i]) {
            printf("error allocating matrix of size %d\n", n);
            exit(0);
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


float **random_matrix(int n)
{
    float **mat = allocate_matrix(n);

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
    float **mat = allocate_matrix(n);

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
    float **mat = allocate_matrix(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = 0;
        }
    }

    return mat;
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
    int num_threads;
};


void print_thread_data(thread_data *d)
{
    printf("thread_data.thread_num: %d\n", d->thread_num);
    printf("thread_data.A: %p\n", d->A);
    printf("thread_data.B: %p\n", d->B);
    printf("thread_data.C: %p\n", d->C);
    printf("thread_data.matrix_size: %d\n", d->matrix_size);
    printf("thread_data.num_threads: %d\n", d->num_threads);
}


void *parallel_mult(void *arg)
{
    thread_data data = *(thread_data *) arg;
    int n = data.matrix_size;
    int id = data.thread_num;
    int num_rows = n / data.num_threads;
    int start_row = id * num_rows;
    float **mat_A = data.A;
    float **mat_B = data.B;
    float **dest = data.C;

    printf("thread #: %d\n", id);

    float sum;
    for (int i = start_row; i < start_row + num_rows; i++) {
        for (int j = 0; j < n; j++) {
            sum = 0.0f;
            for (int k = 0; k < n; k++) {
                sum += mat_A[i][k] * mat_B[j][k];
            }
            dest[i][j] = sum;
        }
    }

    return NULL;
}


void transpose_matrix(float **mat, int n)
{
    float tmp;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            tmp = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = tmp;
        }
    }
}


void *thread_test(void *arg)
{
    thread_data data = *(thread_data *) arg;
    float **m = data.C;

    switch (data.thread_num) {
        case 0:
            print_matrix(data.A, data.matrix_size, "thread 0 printing matrix A\n");
            m[1][2] = 1118;
            break;
        case 1:
            print_matrix(data.B, data.matrix_size, "thread 1 printing matrix B\n");
            data.B[0][0] = 111.1234;
            break;
        case 2:
            print_matrix(data.C, data.matrix_size, "thread 2 printing matrix C\n");
            data.C[0][0] = 99999.1234;
            break;
        default:
            break;
    }

    return NULL;
}


int main(int argc, char *argv[])
{
    int size = DEFAULT_SIZE;
    int num_threads = DEFAULT_THREADS;
    int print_to_console = 0;
    int invalid_args = 0;

    argv++;
    argc--;
    while (argc > 0 && !invalid_args) {
        if (strcmp(argv[0], "-n") == 0 && argv[1] != NULL) {
            argv++;
            size = atoi(argv[0]);
            argc--;
        } else if (strcmp(argv[0], "-t") == 0 && argv[1] != NULL) {
            argv++;
            num_threads = atoi(argv[0]);
            argc--;
        } else if (strcmp(argv[0], "-p") == 0) {
            print_to_console = 1;
        } else {
            invalid_args = 1;
        }
        argv++;
        argc--;
    }

    if (invalid_args) {
        usage();
        exit(0);
    }

    printf("size: %d\n", size);
    printf("threads: %d\n", num_threads);

    float **m_1 = random_matrix(size);
    float **m_2 = ident_matrix(size);
    float **m_3 = empty_matrix(size);

    if (print_to_console) {
        print_matrix(m_1, size, "A:");
        print_matrix(m_2, size, "B:");
    }

    if (num_threads == 1) {
        serial_mult(m_3, m_1, m_2, size);
    } else {
        transpose_matrix(m_2, size);
        pthread_t threads[num_threads];
        thread_data data[num_threads];
        for (int i = 0; i < num_threads; i++) {
            data[i].thread_num = i;
            data[i].A = m_1;
            data[i].B = m_2;
            data[i].C = m_3;
            data[i].matrix_size = size;
            data[i].num_threads = num_threads;
            pthread_create(&threads[i], NULL, parallel_mult, &data[i]);
        }

        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        // calculate remainder
        int remaining_rows = size % num_threads;
        int start = size - remaining_rows;
        for (int i = start; i < size; i++) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    m_3[i][j] += m_1[i][k] * m_2[j][k];
                }
            }
        }
        transpose_matrix(m_2, size);
    }


    if (print_to_console) {
        print_matrix(m_3, size, "C = AxB");
    }

    free_matrix(m_1, size);
    free_matrix(m_2, size);
    free_matrix(m_3, size);

    return 0;
}
