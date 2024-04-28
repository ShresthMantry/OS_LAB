#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER_OF_PHILOSOPHERS 5
#define EATING_TIME 1
#define THINKING_TIME 1

pthread_mutex_t mutexArray[NUMBER_OF_PHILOSOPHERS];

void *diningProblemThread(void *vindex)
{
    int index = *((int *)vindex);
    printf("[%d],arrived\n", index);
    fflush(stdout);

    printf("[%d],think\n", index);
    fflush(stdout);
    sleep(THINKING_TIME);

    while (1)
    {
        // Pick up chopsticks based on odd-even strategy
        if (index % 2 == 0)
        { // Even philosopher
            pthread_mutex_lock(&mutexArray[index]);
            pthread_mutex_lock(&mutexArray[(index + 1) % NUMBER_OF_PHILOSOPHERS]);
        }
        else
        { // Odd philosopher
            pthread_mutex_lock(&mutexArray[(index + 1) % NUMBER_OF_PHILOSOPHERS]);
            pthread_mutex_lock(&mutexArray[index]);
        }

        // Both chopsticks acquired, start eating
        printf("[%d],eating\n", index);
        fflush(stdout);
        sleep(EATING_TIME);

        // Release the chopsticks
        pthread_mutex_unlock(&mutexArray[index]);
        pthread_mutex_unlock(&mutexArray[(index + 1) % NUMBER_OF_PHILOSOPHERS]);
        break; // Break out of the eating loop
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t pidArray[NUMBER_OF_PHILOSOPHERS];

    for (int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++)
    {
        mutexArray[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    }

    for (int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        pthread_create(&pidArray[i], NULL, diningProblemThread, index);
    }

    for (int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++)
    {
        pthread_join(pidArray[i], NULL);
    }

    return 0;
}