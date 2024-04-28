#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>

 int p[2];
pid_t pid1;


void terminateChild(int signum) {
    kill(pid1, SIGTERM);
    printf("Child processes terminated!!!\n");
    exit(0);
} 


int main(int argc, char *argv[])
{
    close(1);
    int fd2 = open("output.txt", O_CREAT | O_WRONLY, 0644);
    dup(fd2);

   
    if (pipe(p) == -1)
    {
        perror("Error creating pipe");
        exit(1);
    }

    pid_t pid1;

    int msg = 16;
    char inbuf[16];


    if ((pid1 = fork()) == 0)
    {
        signal(SIGTERM,terminateChild);
        // close(p[0]);
        // close(1);
        // dup(p[1]);
        // int ch;
        // while((ch=getchar())!=EOF)
        // {
        //     putchar(ch);
        // } 
        close(p[0]);
        dup2(1,p[1]);

        int ch;
        while(1)
        {
            ch = getchar();
            if(ch==EOF)
            {
                raise(SIGTERM);
            }
            putchar(ch);
        }


    }
    else
    {

        // wait(NULL);
        // close(p[1]);
        // close(0);
        // dup(p[0]);
        // int ch;
        // while((ch=getchar())!=EOF)
        // {
        //     putchar(ch);
        // }

        close(p[1]);
        dup2(0,p[0]);

        int ch;

        while(1)
        {
            ch=getchar();
            if(ch==EOF)
            {
                close(p[1]);
                close(p[0]);
                exit(1);
            }
            putchar(ch);
        }

    }

}
