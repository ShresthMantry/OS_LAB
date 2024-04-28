#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>

int p[2];
pid_t p_id1;

void terminateChild(int signum)
{
    kill(p_id1, SIGTERM);
    printf("Child processes terminated!!!\n");
    exit(0);
}

void terminateParent(int signum)
{
    close(p[1]);
    close(p[0]);
    exit(0);
}

int main()
{
    char ch;

    pipe(p);

    if (fork() == 0)
    {
        signal(SIGTERM, terminateChild);

        while (1)
        {
            ch = getchar();
            if (ch == EOF)
            {
                raise(SIGTERM);
            }
            write(p[1], &ch, 1);
        }
    }
    else
    {
        signal(SIGINT, terminateParent);
        close(p[1]);

        while (1)
        {
            if (read(p[0], &ch, 1) > 0)
            {
                // ch--;
                putchar(ch);
            }
            else
            {
                raise(SIGINT);
            }
        }
    }

    printf("Exit");
    exit(1);
}