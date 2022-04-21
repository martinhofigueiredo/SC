#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// A regular C function that executs as a thread
void *myThreadFunction(void *vargp)
{
    int id = *(int *)vargp;
	sleep(2);
	printf("Printing from Thread %d \n", id );
	return NULL;
}

int main()
{
	pthread_t tid_1, tid_2; //Thread ID

	printf("Creating 2 Threads\n");

	pthread_create(&tid_1, NULL, myThreadFunction, (void *)(&tid_1) );
    pthread_create(&tid_2, NULL, myThreadFunction, (void *)(&tid_2) );

	pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);
    
	printf("After Running Threads\n");
	exit(0);
}
