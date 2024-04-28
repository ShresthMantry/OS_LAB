#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

long long sum = 0;

void* sum_runner(void *arg)
{
    long long *limit_ptr = (long long *)arg;
    long long limit = *limit_ptr;
    for(long long i = 0;i<=limit;i++)
    {
        sum = sum+i;
    }

    pthread_exit(0);
}

int main(int argc,char **argv)
{
    if(argc<2)
    {
        printf("Wrong input Format");
        exit(-1);
    }

    long long limit = atoll(argv[1]);

    pthread_t tid;

    // pthread_attr_t attr;
    // pthread_attr_init(&attr);

    pthread_create(&tid,NULL,sum_runner,&limit);
    pthread_join(tid,NULL);

    printf("%lld",sum);



}