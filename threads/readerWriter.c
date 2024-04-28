#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int readcount = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t wrt;

void *writer(void *arg)
{
    int *i = (int *)arg;
    sem_wait(&wrt);
    printf("Writing %d\n", *i);
    sem_post(&wrt);

    pthread_exit(NULL);
}

void *reader(void *arg)
{
    int *i = (int *)arg;
    pthread_mutex_lock(&mutex);
    readcount++;
    if (readcount == 1)
        sem_wait(&wrt);
    pthread_mutex_unlock(&mutex);
    printf("Reading %d No of Readers %d \n", *i, readcount);
    pthread_mutex_lock(&mutex);
    readcount--;
    if (readcount == 0)
        sem_post(&wrt);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main()
{
    // Initialize the semaphores
    sem_init(&wrt, 0, 1);

    pthread_t tids[10];
    for(int i = 0;i<10;i++)
    {
        int *arg = malloc(sizeof(*arg)); 
        *arg = i;

        if(i%2 == 0)
            pthread_create(&tids[i],NULL,reader,arg);
        else
            pthread_create(&tids[i],NULL,writer,arg);
    }

    for(int i = 0;i<10;i++)
    {
        pthread_join(tids[i],NULL);
    }
    
    // Destroy the semaphores
    sem_destroy(&wrt);
    return 0;
}