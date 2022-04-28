#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "buff_helper.h"

#define BUFFER_MAX_SIZE 4

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

unsigned int data_space[BUFFER_MAX_SIZE];
circ_buff_t buffer = { 
    data_space,
    BUFFER_MAX_SIZE,
    0,
    0
};

void* producer(void *arg)
{
    unsigned int push_value;
    while (1) {
        pthread_mutex_lock(&lock);

        push_value = (rand() % 1000); //random [0,999]
        printf("ENTRA PRODUCER\n");
        if (circ_buff_push(&buffer, push_value ) == 0){
            printf("Producer: %u\n", push_value);
            pthread_cond_signal(&cond);} 
        else
            printf("Producer: buffer is full\n");
        printf("SAIU PRODUCER %ld\n", pthread_self());
        pthread_mutex_unlock(&lock);
        usleep(100*1000); //100 ms
    }
    
    return NULL;
}

void* consumer(void *arg)
{
    unsigned int pop_value;
    while (1) {
        pthread_mutex_lock(&lock);
        while(circ_buff_isempty(&buffer) && pthread_cond_wait(&cond, &lock))
        printf("                              ENTRA CONSUMER\n");

        if (circ_buff_pop(&buffer, &pop_value)==0)
            printf("                              Consumer: returned %u\n", pop_value);
        else
            printf("                              Consumer: buffer is empty\n");
        
        pthread_mutex_unlock(&lock);
        printf("                              SAIU CONSUMER %ld\n", pthread_self());
        usleep(150*1000); //150 ms
    }
    
    return NULL;
}
int main(void)
{
    //Seeding...
	srand(time(NULL));

    

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
    printf("Quantas threads produtoras? ");
    int nump;
    scanf("%d", &nump);
    printf("Quantas threads consumidoras? ");
    int numc;
    scanf("%d", &numc);
    printf("Escolheu um totals %d threads\n", nump+numc);
    pthread_t tid[nump+numc];
    for(int i=0; i <= numc ; i++){
        pthread_create(&(tid[i]), NULL, &consumer, NULL);
    }
    for(int i=0; i <= nump ; i++){
        pthread_create(&(tid[i]), NULL, &producer, NULL);
    }
    for(int i=0; i <= nump+numc ; i++){
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}