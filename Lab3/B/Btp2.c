#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Create a global variable to to observe its changes
int g = 0;
 
// A regular C function that executs as a thread
void *myThreadFunction(void *vargp)
{
    int id = *(int *)vargp;
	
	// Create a static variable to observe its changes
    static int s = 0;
  
    // Change static and global variables
    s++; g++;

	//Change again and print
    printf("Thread ID(%p): %d, Static(%p): %d, Global(%p): %d\n", 
                      &id, id,        &s, ++s,        &g, ++g);

	return NULL;
}

int main()
{
	pthread_t tid[3]; //Thread ID

	for (int i = 0; i < 3; i++){
        pthread_create(&tid[i], NULL, myThreadFunction, (void *)&tid[i]);
        usleep(100);
	}
    
    for (int i = 0; i < 3; i++)
        pthread_join(tid[i], NULL);

	exit(0);
}
