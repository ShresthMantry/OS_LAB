#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int size = 0;

void merge(int *arr,int low,int mid,int high)
{
    int left = low;
    int right = mid+1;
    int *temp = (int*)calloc(high-low+1,sizeof(int));

    int k = 0;
    while(left<=mid && right<=high)
    {
        if(arr[left]<=arr[right])
        {
            temp[k++]=arr[left++];
        }
        else if(arr[right]<arr[left])
        {
            temp[k++]=arr[right++];
        }
    }

    while(left<=mid)
    {
        temp[k++]=arr[left++];
    }
    while(right<=high)
    {
        temp[k++]=arr[right++];
    }

    for(int i = low;i<=high;i++)
    {
        arr[i]=temp[i-low];
    }
    free(temp);
}

void *selectionSort(void *arg)
{
    int *arr = (int *)arg;
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
    int arr1[n / 2];
    int arr2[n / 2];

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    for (int i = 0; i < n / 2; i++)
    {
        arr1[i] = arr[i];
    }

    for (int i = n / 2; i < n; i++)
    {
        arr2[i - n / 2] = arr[i];
    }

    pthread_t tid1, tid2;

    size = n / 2;
    pthread_create(&tid1, NULL, selectionSort, arr1);
    pthread_create(&tid2, NULL, selectionSort, arr2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    int *temp = (int*)calloc(n,sizeof(int));

    for(int i = 0;i<n/2;i++)
    {
        temp[i]=arr1[i];
    }

    for(int i = n/2;i<n;i++)
    {
        temp[i]=arr2[i-n/2];
    }
    int left = 0;
    int mid = n/2;
    int right = mid;
    int high = n;

    int k = 0;
    while(left<mid && right<high)
    {
        if(arr1[left]<=arr2[right])
        {
            temp[k++]=arr1[left++];
        }
        else if(arr2[right]<arr1[left])
        {
            temp[k++]=arr2[right++];
        }
    }

    while(left<mid)
    {
        temp[k++]=arr1[left++];
    }
    while(right<high)
    {
        temp[k++]=arr2[right++];
    }

    
    // merge(temp,0,n/2,n);

    for(int i = 0;i<n;i++)
    {
        printf("%d ",temp[i]);
    }
}