#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

void terminal_clear() {
    printf("\033[2J"); fflush(stdout);
}

void terminal_print_at(int line, const char *st) {
    printf("\033[%d;%dH", line, 1); fflush(stdout);
    puts(st); fflush(stdout);
}

pthread_mutex_t lock;

void* printer(void *arg) {
    
	//Pick a pseudo-random line [1..10]
	int line = (long)pthread_self()%10 + 1;

    while(1) {
        pthread_mutex_lock(&lock);
        
        terminal_print_at(line, arg);

        pthread_mutex_unlock(&lock);
        usleep(100*1000); //100 ms
    }

    return NULL;
}


int main(void)
{
    pthread_t tid[3];

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    terminal_clear();

	pthread_create(&(tid[0]), NULL, &printer, "I'm a very long long long long long long long long long long long long long long string");
    pthread_create(&(tid[1]), NULL, &printer, "short string 1");
    pthread_create(&(tid[2]), NULL, &printer, "short string 2");

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}