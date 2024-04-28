#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{
    int *arr;
    int size;
} parameters;

void merge(int *arr, int low, int mid, int high)
{
    int left = low;
    int right = mid;
    int *temp = (int *)calloc(high - low + 1, sizeof(int));

    int k = 0;
    while (left < mid && right < high)
    {
        if (arr[left] <= arr[right])
        {
            temp[k++] = arr[left++];
        }
        else if (arr[right] < arr[left])
        {
            temp[k++] = arr[right++];
        }
    }

    while (left < mid)
    {
        temp[k++] = arr[left++];
    }
    while (right < high)
    {
        temp[k++] = arr[right++];
    }

    for (int i = low; i < high; i++)
    {
        arr[i] = temp[i - low];
    }
    free(temp);
}

void *selectionSort(void *arg)
{
    parameters *para = (parameters*)arg;
    int size = para->size;
    int *arr = para->arr;

    for (int i = 0; i < size; i++)
    {
        int mini = i;
        for (int j = i; j < size; j++)
        {
            if (arr[j] < arr[mini])
            {
                mini = j;
            }
        }
        int temp = arr[i];
        arr[i] = arr[mini];
        arr[mini] = temp;
    }

    pthread_exit(NULL);
}

int main()
{
    int n;
    printf("Enter n\n");
    scanf("%d", &n);

    int *arr = (int *)calloc(n, sizeof(int));
    int *arr1;
    int *arr2;

    if(n%2==0)
    {
        arr1 = (int *)calloc(n/2, sizeof(int));
        arr2 = (int *)calloc(n/2, sizeof(int));
    }
    else
    {
        arr1 = (int *)calloc(n/2+1, sizeof(int));
        arr2 = (int *)calloc(n/2, sizeof(int));
    }

    int size = n%2==0?n/2:n/2+1;

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    for (int i = 0; i < size; i++)
    {
        arr1[i] = arr[i];
    }

    for (int i = size; i < n; i++)
    {
        arr2[i - size] = arr[i];
    }

    pthread_t tid1, tid2;
    parameters * para1 = (parameters*)calloc(1,sizeof(parameters));
    parameters * para2 = (parameters*)calloc(1,sizeof(parameters));

    para1->arr = arr1;
    para2->arr = arr2;
    para1->size = size;
    para2->size = n%2==0?size:size-1;



    pthread_create(&tid1, NULL, selectionSort, para1);
    pthread_create(&tid2, NULL, selectionSort, para2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    int *temp = (int*)calloc(n,sizeof(int));

    for(int i = 0;i<size;i++)
    {
        temp[i]=arr1[i];
    }

    for(int i = size;i<n;i++)
    {
        temp[i]=arr2[i-size];
    }
    
    merge(temp,0,size,n);

    for(int i = 0;i<n;i++)
    {
        printf("%d ",temp[i]);
    }
}