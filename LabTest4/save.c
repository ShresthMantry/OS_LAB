#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_SIZE 100

char Input[MAX_SIZE];
char Output[MAX_SIZE];
int current_index = 0;
int n;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *caseInterChange(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        char ch = Input[current_index++];
        if (ch== '\0') {
            pthread_mutex_unlock(&lock);
            break;
        }
        if(ch>='A' && ch<='Z')
        {
            Output[current_index-1]=(char)(ch-'A'+'a');
        }
        else if(ch>='a' && ch <='z')
        {
            Output[current_index-1]=(char)(ch-'a'+'A');
        }
        else
        {
            Output[current_index-1]=ch;
        }
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

int main()
{
    printf("Enter a sentence to convert: ");
    fgets(Input, MAX_SIZE, stdin);
    
    printf("Enter the number of threads to be used: ");
    scanf("%d", &n);

    pthread_t threads[n];
    
    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, caseInterChange, NULL);
    }
    
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Converted sentence is as follows: %s\n", Output);
    
    return 0;

}