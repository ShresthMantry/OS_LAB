#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty, full;

void *producer(void *arg)
{
    int item;
    while (1)
    {
        item = rand() % 100; // Generate random data item

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[count++] = item;
        printf("Produced: %d\n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        // Simulate some work done by the producer
        sleep(1);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    int item;
    while (1)
    {

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (count > 0)
        { // Check if there is something in the buffer
            item = buffer[--count];
            printf("Consumed: %d\n", item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        // Simulate some work done by the consumer
        sleep(1);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t producer_thread, consumer_thread;

    // Initialize the semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for the threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy the semaphores
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}