#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

struct sum_runner_struct {
    long long limit;
    long long answer;
};

void* sum_runner(void *arg)
{
    long long sum = 0;
    struct sum_runner_struct* limit = (struct sum_runner_struct*)arg;
    for(long long i = 0;i<=limit->limit;i++)
    {
        sum = sum+i;
    }

    limit->answer=sum;
    pthread_exit(0);
}

int main(int argc,char **argv)
{
    if(argc<2)
    {
        printf("Wrong input Format");
        exit(-1);
    }

    int num_args = argc-1;
    pthread_t tids[num_args];
    struct sum_runner_struct totals[num_args];


    for(int i = 0;i<num_args;i++)
    {
        long long limit = atoll(argv[i+1]);
        totals[i].limit=limit;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tids[i],&attr,sum_runner,&totals[i]);
    }
    
    for(int i=0;i<num_args;i++)
    {
        pthread_join(tids[i],NULL);
        printf("Sum is %lld \n",totals[i].answer);
    }
   



}