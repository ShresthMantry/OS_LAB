#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0;
sem_t empty, full,mutex;

void *producer(void *arg)
{
    int item = rand() % 100;
    sem_wait(&empty);
    sem_wait(&mutex);
    buffer[count++] = item;
    printf("Produced: %d\n", item);
    sem_post(&mutex);
    sem_post(&full);


    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    int item;
    sem_wait(&full);
    sem_wait(&mutex);
    if(count>0)
    {
        item = buffer[--count];
        printf("Consumed: %d\n", item);
    }
    sem_post(&mutex);
    sem_post(&empty);

    pthread_exit(NULL);
}

int main()
{
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t tids[20];
    for (int i = 0; i < 20; i++)
    {
        if (i % 2 == 0)
            pthread_create(&tids[i], NULL, consumer, NULL);
        else
            pthread_create(&tids[i], NULL, producer, NULL);
    }

    for (int i = 0; i < 20; i++)
        pthread_join(tids[i], NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    return 0;
}