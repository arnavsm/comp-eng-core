#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Value depend on System core
#define CORE 4

// Maximum matrix size
#define MAX 4

// Maximum threads is equal to total core of system
pthread_t thread[CORE * 2];
int mat_A[MAX][MAX], mat_B[MAX][MAX], sum[MAX][MAX],
    sub[MAX][MAX];

// Addition of a Matrix
void* addition(void* arg)
{

    int i, j;
    int core = (int)arg;

    // Each thread computes 1/4th of matrix addition
    for (i = core * MAX / 4; i < (core + 1) * MAX / 4;
         i++) {

        for (j = 0; j < MAX; j++) {

            // Compute Sum Row wise
            sum[i][j] = mat_A[i][j] + mat_B[i][j];
        }
    }
}

// Driver Code
int main()
{
    int i, j, step = 0;
    // Generating random values in mat_A and mat_B
    for (i = 0; i < MAX; i++) {

        for (j = 0; j < MAX; j++) {

            mat_A[i][j] = rand() % 10;
            mat_B[i][j] = rand() % 10;
        }
    }

    // Displaying mat_A
    printf("\nMatrix A:\n");

    for (i = 0; i < MAX; i++) {

        for (j = 0; j < MAX; j++) {

            printf("%d ", mat_A[i][j]);
        }

        printf("\n");
    }

    // Displaying mat_B
    printf("\nMatrix B:\n");

    for (i = 0; i < MAX; i++) {

        for (j = 0; j < MAX; j++) {

            printf("%d ", mat_B[i][j]);
        }

        printf("\n");
    }

    // Creating threads equal
    // to core size and compute matrix row
    for (i = 0; i < CORE; i++) {

        pthread_create(&thread[i], NULL, &addition,
                       (void*)step);
        step++;
    }

    // Waiting for join threads after compute
    for (i = 0; i < CORE * 2; i++) {

        pthread_join(thread[i], NULL);
    }

    // Display Addition of mat_A and mat_B
    printf("\n Sum of Matrix A and B:\n");

    for (i = 0; i < MAX; i++) {

        for (j = 0; j < MAX; j++) {

            printf("%d ", sum[i][j]);
        }

        printf("\n");
    }
    return 0;
}

