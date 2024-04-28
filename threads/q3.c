#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void* sum_runner(void *arg)
{
    long long sum = 0;
    long long *limit_ptr = (long long *)arg;
    long long limit = *limit_ptr;
    for(long long i = 0;i<=limit;i++)
    {
        sum = sum+i;
    }

    long long *result = malloc(sizeof(*result));
    *result = sum;
    pthread_exit(result);
}

int main(int argc,char **argv)
{
    if(argc<2)
    {
        printf("Wrong input Format");
        exit(-1);
    }

    long long limit = atoll(argv[1]);
    long long *result = malloc(sizeof(*result));

    pthread_t tid;

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid,&attr,sum_runner,&limit);
    pthread_join(tid,(void**)&result);

    printf("%lld",*result);
    free(result);



}