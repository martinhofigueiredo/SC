#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "buff_helper.h"

#define BUFFER_MAX_SIZE 4

unsigned int data_space1[BUFFER_MAX_SIZE];
circ_buff_t buffer1 = { 
    data_space1,
    BUFFER_MAX_SIZE,
    0,
    0
};

unsigned int data_space2[BUFFER_MAX_SIZE];
circ_buff_t buffer2 = { 
    data_space2,
    BUFFER_MAX_SIZE,
    0,
    0
};

pthread_mutex_t buff1_mutex, buff2_mutex;
pthread_cond_t buff1_prod_condv, buff2_prod_condv;

int suspend_f = 0;

void* producer(void *arg)
{
    unsigned int push_value;
    while (1) {
        push_value = (rand() % 1000); //random [0,999]
        if ( pthread_mutex_lock(&buff1_mutex) == 0 ) { ///bem sucedido tem o mutex
            
            if (circ_buff_push(&buffer1, push_value ) == 0){
                printf("Producer: %u\n", push_value);
            }
            else{
                pthread_cond_wait(&buff1_prod_condv)
                printf("Producer: buffer is full\n"); 
            }
            pthread_mutex_unlock(&buff1_mutex);
        }
        
        usleep(100*1000); //100 ms
    }
    
    return NULL;
}

void* relay(void *arg)
{
    unsigned int pop_value;

    while (1) {
        if ( pthread_mutex_lock(&buff1_mutex) == 0 ) {
           
            if ( pthread_mutex_lock(&buff2_mutex) == 0 ) {
                
                if (circ_buff_pop(&buffer1, &pop_value)==0) {
                    //Success popping from 1
                    pthread_cond_signal(&buff1_prod_condv)
                    if (circ_buff_push(&buffer2, pop_value ) == 0){
                        pthread_cond_signal(&buff2_prod_condv)
                    } else {
                        //Push to Buff2 failed - Full
                        //Will drop the packet (Solution2)
                        fprintf(stderr, "Buffer 2 is Full - Droped value: %d\n", pop_value);
                    }
                }
                
                pthread_mutex_unlock(&buff2_mutex);
            }            

            pthread_mutex_unlock(&buff1_mutex);
        }  

    }
    
    return NULL;
}


void* consumer(void *arg)
{
    unsigned int pop_value;
    while (1) {
        
        if (suspend_f)
            continue; //This will busy-wait (CPU-100%)

        if ( pthread_mutex_lock(&buff2_mutex) == 0 ) {
           
            if (circ_buff_pop(&buffer2, &pop_value)==0)
                printf("                              Consumer %ld: returned %u\n", pthread_self(), pop_value);
            else{
                pthread_cond_wait(&buff2_prod_condv);
                printf("                              Consumer: buffer is empty\n");
            }
            pthread_mutex_unlock(&buff2_mutex);
        }  
        
    }
    
    return NULL;
}

void* cli(void *arg)
{
    while (1) {
        getchar(); //Dont care about the char
        suspend_f = !suspend_f;
        char *string[3];
        strcpy(string, (suspend_f ? "Yes":"No "));
        printf ("Suspend: %s\n", string);
    }
    
    return NULL;
}


int main(void)
{
    //Seeding...
	srand(time(NULL));

    pthread_t tid[4];

    if (pthread_mutex_init(&buff1_mutex, NULL) !=0 ){
        fprintf(stderr, "Error in pthread_mutex_init ");
        return 1;
    }

    if (pthread_cond_init(&buff1_prod_condv, NULL) !=0 ){
        fprintf(stderr, "Error in pthread_cond_init ");
        return 1;
    }

    if (pthread_mutex_init(&buff2_mutex, NULL) !=0 ){
        fprintf(stderr, "Error in pthread_mutex_init ");
        return 1;
    }

    if (pthread_cond_init(&buff2_prod_condv, NULL) !=0 ){
        fprintf(stderr, "Error in pthread_cond_init ");
        return 1;
    }
    
	pthread_create(&(tid[0]), NULL, &producer, NULL);
	pthread_create(&(tid[1]), NULL, &relay, NULL); 
    pthread_create(&(tid[2]), NULL, &consumer, NULL);
    pthread_create(&(tid[3]), NULL, &cli, NULL);

    for (int i = 0; i < 4; i++)
        pthread_join(tid[i], NULL);

    pthread_cond_destroy(&buff1_prod_condv);
    pthread_mutex_destroy(&buff1_mutex);

    pthread_cond_destroy(&buff2_prod_condv);
    pthread_mutex_destroy(&buff2_mutex);

    return 0;
}