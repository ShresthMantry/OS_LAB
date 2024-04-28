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
            close(p1[1]);
            close(p2[1]);

            dup2(0, p2[0]);

            while (1)
            {
                if ((ch = getchar()) != EOF)
                {
                    ch--;
                    putchar(ch);
                }
                else
                {
                    close(p1[1]);
                    close(p2[0]);
                    close(p2[1]);
                    exit(0);
                }
            }
        }
        else
        {
            close(p1[1]);
            close(p2[0]);

            dup2(0, p1[0]);
            dup2(1, p2[1]);


            while (1)
            {
                ch = getchar();
                if (ch == EOF)
                {
                    kill(p_id2, SIGTERM);
                    printf("Child processes terminated!!!\n");
                    exit(1);
                }
                ch++;
                putchar(ch);
            }
        }
    }
    else
    {
        close(p1[0]);

        dup2(1, p1[1]);


        while (1)
        {
            ch = getchar();
            if (ch == EOF)
            {
                kill(p_id1, SIGTERM);
                printf("Child processes terminated!!!\n");
                exit(1);
            }
            putchar(ch);
        }
    }
}