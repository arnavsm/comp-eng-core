#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define m 3
#define n 2

int A[m][n], B[m][n], SUM[m][n];
pthread_t thread[m][n];

struct arg_struct { 
    int row;
    int column;
};

void *addition(void *args) {
    struct arg_struct *index = (struct arg_struct *)args;
    int row = index->row;
    int column = index->column;
    
    printf("Inside Thread (Row: %d, Column: %d)\n", row, column);
    SUM[row][column] = A[row][column] + B[row][column];
    free(args);
}

int main() {
    // Input
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("For A, enter number at (%d, %d) index: ", i, j);
            scanf("%d", &A[i][j]);
            printf("For B, enter number at (%d, %d) index: ", i, j);
            scanf("%d", &B[i][j]);
        }
    }

    // Displaying A
    printf("\nMatrix A:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    // Displaying B
    printf("\nMatrix B:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }
    
    printf("\n");

    // Performing Addition
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            struct arg_struct *args = malloc(sizeof(struct arg_struct));
            args->row = i;
            args->column = j;
            pthread_create(&thread[i][j], NULL, &addition, (void *)args);
        }
    }
    
    // Waiting for Threads to Finish
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            pthread_join(thread[i][j], NULL);
        }
    }
    
    // Display Addition of A and B
    printf("\nSum of Matrix A and B:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", SUM[i][j]);
        }
        printf("\n");
    }
    return 0;
}

