#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>
int status_count, status_convert;
jmp_buf env;

void alarm_handler(int signum)
{
    fprintf(stderr, "Read timeout in the second child, killing both children.\n");
    kill(0, SIGTERM); // Kill all processes in the group
    longjmp(env, 1);  // Jump back to the setjmp point
}

int caseDescp(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("type on keyboard, watch screen");
        exit(1);
    }
    else if (argc == 2)
    {
        if (atoi(argv[1]) > 0 || atoi(argv[1]) < 0)
        {
            fprintf(stderr, "There is only optional arg no file");
            exit(1);
        }
        else
        {
            return 1; // only read 1 file
        }
    }
    else if (argc == 3)
    {
        if (atoi(argv[1]) > 0 || atoi(argv[1]) < 0)
        {
            return 2; // arg + file
        }

        else
        {
            return 3; // normal state
        }
    }
    else if (argc == 4)
    {
        // if (atoi(argv[1]) == 0)
        // {
        //     fprintf(stderr, "There is optional argument error");
        //     exit(1);
        // }
        // else
        return 4; // all provided
    }
}

int main(int argc, char *argv[])
{   //convert2.c and countTry.c
    int fd1;
    int flag = caseDescp(argc, argv);
    fprintf(stderr,"case %d\n",flag);
    
    if(flag==1){
          fd1 = open(argv[1], O_RDONLY);
          close(0);
          dup(fd1);
         const char *printing = "The contents of the file supplied are:\n";
    write(1, printing, strlen(printing));
    char buff[BUFSIZ];
    int n=read(0,buff,BUFSIZ);
    write(1,buff,n);
    exit(0);
    }


    if (flag == 3 || flag == 1)
        fd1 = open(argv[1], O_RDONLY);
    else
        fd1 = open(argv[2], O_RDONLY);
    if (fd1 == -1)
    {
        perror("Error opening reading file");
        exit(1);
    }
    close(0);
    if (dup(fd1) == -1)
    {
        perror("Error duplicating file descriptor");
        exit(1);
    }

    int fd2;
    if (flag == 3)
        fd2 = open(argv[2], O_CREAT | O_WRONLY, 0644);
    else if (flag == 4)
        fd2 = open(argv[3], O_CREAT | O_WRONLY, 0644);
    if (fd2 == -1 && flag != 2)
    {
        perror("Error opening or creating output file");
        exit(1);
    }
    close(1);
    if (dup(fd2) == -1 && flag != 2)
    {
        perror("Error duplicating file descriptor");
        exit(1);
    }

    int p[2];
    if (pipe(p) == -1)
    {
        perror("Error creating pipe");
        exit(1);
    }

    signal(SIGALRM, alarm_handler);

    if (setjmp(env) == 0)
    {
        alarm(15); // Set an alarm to go off in 15 seconds

        pid_t pid_count = fork();
        if (pid_count != 0)
        // {   setpgid(0,0);
        {
            pid_t pid_convert = fork();
            if (pid_convert != 0)
            {
                close(p[1]);
                close(p[0]);

                
                pid_t wpid;

                while ((wpid = wait(&status_count)) > 0)
                {
                    fprintf(stderr, "Child PID=%d reaped with exit status=%d\n", wpid, WEXITSTATUS(status_count));

                    // Check exit status of count process
                    // if (wpid == pid_count && WEXITSTATUS(status_count) == 2)
                    // {
                    //     fprintf(stderr, "Count process exited with status 2. Killing convert process.\n");
                         
                    //     // kill(pid_convert, SIGTERM);
                    //     kill(pid_convert,SIGTERM);
                    //     // sleep(5);
                    //     // kill(pid_convert,SIGKILL);
                    // }
                }

                while ((wpid = wait(&status_convert)) > 0)
                {
                    fprintf(stderr, "Child PID=%d reaped with exit status=%d\n", wpid, WEXITSTATUS(status_convert));
                }
            }
            else
            {    close(p[1]);
            close(0);
            if (dup(p[0]) == -1)
            {
                perror("Error duplicating file descriptor");
                exit(1);
            }

            pid_t pid = getpid();
            fprintf(stderr, "Process ID  (PID) of child doing count: %d\n", pid);

            if (flag == 4 || flag == 2)
            {
                execl("./count", "count", argv[1], (char *)0);
            }
            else
            {
                execl("./count", "count", (char *)0);
            }
            perror("Error executing count");
            exit(1);
                
            }
        }
        else
        {
            // c1 (count) process
            // c2 (convert) process
                close(p[0]);
                close(1);
                if (dup(p[1]) == -1)
                {
                    perror("Error duplicating file descriptor");
                    exit(1);
                }
                pid_t pid = getpid();
                fprintf(stderr, "Process ID (PID) of child doing convert: %d\n", pid);
                // sleep(6);
                execl("./convert", "convert", (char *)0);
                perror("Error executing convert");
                exit(1);
        }
    }
    else
    {
        wait(NULL);
        wait(NULL);
        exit(1);
    }
    if(WEXITSTATUS(status_count)==0&&WEXITSTATUS(status_convert)==0)
    {fprintf(stderr, "Normal children exit\n");
    exit(0);}
}