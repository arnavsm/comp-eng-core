#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define m 3
#define n 2

int A[m][n], B[m][n], SUM[m][n];
pthread_t thread[m];

void *addition(void *arg) {
	int row = (int)arg;
 	printf("Inside Thread/Row: %d\n", row);
 	for(int j = 0; j < n; j++) {
 		SUM[row][j] = A[row][j] + B[row][j];
 	}
}


int main() {
	// Input
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			int temp;
			printf("For A, enter number at (%d, %d) index: ", i, j);
			scanf("%d", &temp);
			A[i][j] = temp;
			printf("For B, enter number at (%d, %d) index: ", i, j);
			scanf("%d", &temp);
			B[i][j] = temp;
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
    int row = 0;
    // Creating Thread
    for (int i = 0; i < m; i++) {
    	pthread_create(&thread[i], NULL, &addition, (int *)row);
    	row++;
    }
    // Waiting for Thread to Finish
    for (int i = 0; i < m; i++) {
    	pthread_join(thread[i], NULL);
    }
	
	
	
	// Display Addition of mA and mB
    printf("\nSum of Matrix A and B:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", SUM[i][j]);
        }
        printf("\n");
    }
    return 0;
}
