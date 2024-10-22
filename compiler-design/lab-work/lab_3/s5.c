#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
     int SIZE;
     SIZE = rand()%10;
     srand (time(NULL));

    int martix[SIZE][SIZE];
    
    for (int i=0;i<SIZE;i++)
   {
         for(int j=0;j<SIZE;j++)
          {
            martix[i][j] = rand() %100;
          }
 }
 	printf(" martix of the random numbers : \n");
   	for (int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			printf("%4d",martix[i][j]);
		}
		printf("\n");
	}
  int sum =0;
     for (int i=0;i<SIZE;i++)
     {
        sum+=martix[i][i];
     }
  printf("sum of diagonal elements is =%d\n", sum); 
  
     return 0;
}