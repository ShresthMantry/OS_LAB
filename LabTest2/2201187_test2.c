#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf env;
pid_t pid1;

void sigtermHandler(int signo)
{
    kill(pid1, SIGTERM);
    printf("Calculation is taking too much time.\n");
}

void alarmHandler()
{
    longjmp(env, 1);
}

int primeNumber(int n)
{
    int count = 0;
    for (int i = 1; i <= n; i++)
    {
        if (n % i == 0)
        {
            count++;
        }
    }
    if (count == 2)
    {
        return 1;
    }
    return 0;
}

void primes(int x, int y)
{
    int i = x;
    while (i <= y)
    {
        if (primeNumber(i) == 1)
        {
            printf("%d \n", i);
        }
        i = i + 1;
    }
}

int main()
{

    int x, y, z;
    printf("Enter x and y and z\n");
    scanf("%d %d %d", &x, &y, &z);

    if ((pid1 = fork()) != 0)
    {
        signal(SIGALRM, alarmHandler);
        signal(SIGTERM, sigtermHandler);
        if (setjmp(env) != 0)
        {
            raise(SIGTERM);
        }
        else
        {
            alarm(z);
            int status;
            wait(&status);
            if (WIFEXITED(status))
            {
                printf("All the prime numbers printed.\n");
            }
            else
            {
                printf("Child process was terminated by force.\n");
            }
        }
    }
    else
    {
        primes(x, y);
        printf("Child completed\n");
        exit(1);
    }
}




