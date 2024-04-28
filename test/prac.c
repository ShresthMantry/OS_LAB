#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>

int p1[2], p2[2];
pid_t p_id1, p_id2;

void terminateChild(int signum)
{
    kill(p_id1, SIGTERM);
    kill(p_id2, SIGTERM);
    printf("Child processes terminated!!!\n");
    exit(0);
}

void terminateParent(int signum)
{
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    exit(0);
}

int main()
{
    char ch;

    if (pipe(p1) == -1 || pipe(p2) == -1)
    {
        fprintf(stderr, "error in pipe creation");
        exit(1);
    }

    if ((p_id1 = fork()) != 0)
    {
        if ((p_id2 = fork()) != 0)
        {
            signal(SIGINT, terminateParent);
            close(p1[1]);
            close(p2[1]);

            while (1)
            {
                if (read(p2[0], &ch, 1) > 0)
                {
                    ch--;
                    putchar(ch);
                }
                else
                {
                    raise(SIGINT);
                }
            }
        }
        else
        {
            close(p1[1]);
            close(p2[0]);
            signal(SIGTERM, terminateChild);

            while (1)
            {
                read(p1[0], &ch, 1);
                if (ch == EOF)
                {
                    raise(SIGTERM);
                }
                ch++;
                write(p2[1], &ch, 1);
            }
        }
    }
    else{
        close(p1[0]);
        signal(SIGTERM, terminateChild);

        while (1) {
            ch = getchar();
            if (ch == EOF) {
                raise(SIGTERM);
            }
            write(p1[1], &ch, 1);
        }
    }
}