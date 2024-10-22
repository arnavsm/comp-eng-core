
//Program to create a thread. The thread is passed more than one input from the main process. For passing multiple inputs we need to create structure and include all the variables that are to be passed in this structure.
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
struct arg_struct {   //structure which contains multiple variables that are to passed as input to the thread
     int arg1;
     int arg2;
};
 
void *arguments(void *arguments)
{
    struct arg_struct *args=arguments;
    int *sum = malloc(sizeof(int));
    printf("%d\n", args -> arg1);
    printf("%d\n", args -> arg2);
    *sum=args -> arg1 + args -> arg2;
    printf("Sum = %d\n", *sum); 
    pthread_exit(sum);
}

int main()
{
     pthread_t t;
     int *sum;
     struct arg_struct args;
     args.arg1 = 5;
     args.arg2 = 7;
     pthread_create(&t, NULL, arguments,(void *)&args); 
//structure passed as 4th argument
     pthread_join(t, (void **)&sum); /* Wait until thread is finished */
     printf("Sum returned by thread = %d\n", *sum);
}
