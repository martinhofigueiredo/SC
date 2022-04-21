#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define TASKS_NUM 100000

// A regular C function that executes as a thread
void *myThreadFunction(void *vargp)
{
	return NULL;
}

int main()
{
	int ret;
	pthread_t tid; //Thread ID

	struct timespec start, end;

	//Launch TASKS_NUM processes
	printf("Running %d dummy processes...\n", TASKS_NUM);

	clock_gettime(CLOCK_REALTIME, &start);

	for (int i=0; i<TASKS_NUM; i++){
		if ( (ret=fork()) == 0){
			// CHILD!
			myThreadFunction(NULL);
			exit(0);
		}else{
			//The PARENT
			wait(NULL); //Wait on the process
		}
	}

	clock_gettime(CLOCK_REALTIME, &end);

	printf("Handled %d Processes in %ld ms!\n\n", TASKS_NUM, (end.tv_sec-start.tv_sec)*1000 + (end.tv_nsec-start.tv_nsec)/1000000 );

	printf("Running %d dummy threads...\n", TASKS_NUM);
	
	clock_gettime(CLOCK_REALTIME, &start);
	
	for (int i=0; i<TASKS_NUM; i++){
		pthread_create(&tid, NULL, myThreadFunction, NULL );
		pthread_join(tid, NULL);
	}

	clock_gettime(CLOCK_REALTIME, &end);

	printf("Handled %d Threads in %ld ms!\n\n", TASKS_NUM, (end.tv_sec-start.tv_sec)*1000 + (end.tv_nsec-start.tv_nsec)/1000000 );
	exit(0);
}
