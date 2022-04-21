#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

int counter;
pthread_mutex_t counter_lock;

int mod_value;
pthread_mutex_t mod_value_lock;

void* increment(void *arg)
{
    while(1){
        pthread_mutex_lock(&counter_lock);
        counter ++;
        
        if (counter%10 == 0){
            pthread_mutex_lock(&mod_value_lock);
            mod_value ++;
            usleep(30*1000); //Simulate some heavy computation
            pthread_mutex_unlock(&mod_value_lock);
        }

        pthread_mutex_unlock(&counter_lock);

        printf("\n Increment counter:%d   mod_value:%d\n", counter, mod_value);
        
        usleep(50*1000); //50 ms
    }

    return NULL;
}

void* fastincrement(void *arg)
{
    while(1){
        pthread_mutex_lock(&mod_value_lock);
        pthread_mutex_lock(&counter_lock);
        counter ++;
        mod_value += 10;
        //usleep(300*1000); //Simulate some heavy computation
        pthread_mutex_unlock(&counter_lock);
        pthread_mutex_unlock(&mod_value_lock);

        printf("\n FastIncrement counter:%d   mod_value:%d\n", counter, mod_value);
        
        usleep(50*1000); //50 ms
    }

    return NULL;
}

int main(void)
{
    pthread_t tid[2];

    if (pthread_mutex_init(&counter_lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }

    if (pthread_mutex_init(&mod_value_lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }

	pthread_create(&(tid[0]), NULL, &increment, NULL);
	pthread_create(&(tid[1]), NULL, &fastincrement, NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    pthread_mutex_destroy(&counter_lock);
    pthread_mutex_destroy(&mod_value_lock);

    return 0;
}