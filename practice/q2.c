#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    close(0);
    int fd1 = open("file.txt", O_RDONLY);
    dup(fd1);

    close(1);
    int fd2 = open("output.txt", O_CREAT | O_WRONLY, 0644);
    dup(fd2);

    int p[2];
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
        wait(NULL);
        close(p[1]);
        close(0);
        dup(p[0]);
        int ch;
        while((ch=getchar())!=EOF)
        {
            putchar(ch);
        }

        
    }
    else
    {

        close(p[0]);
        close(1);
        dup(p[1]);
        int ch;
        while((ch=getchar())!=EOF)
        {
            putchar(ch);
        } 

    }

}
