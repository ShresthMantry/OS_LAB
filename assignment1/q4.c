#include "stdio.h"
#include "fcntl.h"

#include <unistd.h>

#define BUFFER_SIZE 1024
int main() {
    printf("Trying open() func\n");
    int fd = open("example1.txt", O_RDONLY | O_CREAT, 0644);
    printf("The file status is: %d\n", fd);
    printf("trying out creat() func\n");
    int fd1 = creat("example2.txt", 0644);
    printf("The file status is: %d\n", fd1);

    int pipefd[2];
    pipe(pipefd);
    printf("pipe: Read file descriptor: %d, Write file descriptor: %d\n", pipefd[0], pipefd[1]);

    int newfd = dup(fd);
    printf("New fd is %d\n", newfd);
    int newfd1 = dup2(fd1,100);
    printf("New fd is %d\n",newfd1);
    char buff[BUFFER_SIZE];
    int readSize = read(fd,buff,BUFFER_SIZE);
    write(fd1,buff,readSize);
    printf("Closing Files\n");
    close(fd);
    close(fd1);

    return 0;
}