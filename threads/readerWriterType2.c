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
    for(int i = 1;i<=20;i++)
    {
        sem_wait(&wrt);
        printf("Writing %d\n",i);
        sem_post(&wrt);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *reader(void *arg)
{
    for(int i = 1;i<=20;i++)
    {
        pthread_mutex_lock(&mutex);
        readcount++;
        if(readcount == 1)
            sem_wait(&wrt);
        pthread_mutex_unlock(&mutex);
        printf("Reading %d No of Readers %d \n",i,readcount);
        pthread_mutex_lock(&mutex);
        readcount--;
        if(readcount == 0)
            sem_post(&wrt);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}



int main()
{
    // Initialize the semaphores
    sem_init(&wrt, 0, 1);

    pthread_t reader_thread, writer_thread;
    // Create producer and consumer threads
    pthread_create(&reader_thread, NULL, reader, NULL);
    pthread_create(&writer_thread, NULL, writer, NULL);

    // Wait for the threads to finish
    pthread_join(reader_thread, NULL);
    pthread_join(writer_thread, NULL);


    // Destroy the semaphores
    sem_destroy(&wrt);
    return 0;
}