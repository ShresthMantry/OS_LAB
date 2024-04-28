#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_PROCESS 100
#define INF 999999

typedef struct
{
    char *name;
    int arrival;
    int cpuburst;
    int remaining_burst;
    int turnaround;
    int wait;
} Process;

typedef struct queue
{
    int front, rear;
    unsigned capacity;
    int *arr;
} Queue_t, *Queue;

Queue newQueue(unsigned capacity)
{

    Queue q = (Queue)calloc(1, sizeof(Queue_t));
    q->front = -1;
    q->capacity = capacity;
    q->rear = -1;
    q->arr = (int *)calloc(capacity, sizeof(int));
    // printf("done");

    return q;
}

void push(Queue q, int data)
{
    if ((q->rear == (q->capacity - 1) && q->front == 0) || ((q->front - 1) == q->rear))
    {
        return;
    }

    if (q->front == -1)
    {
        q->front = 0;
        q->rear = 0;
        q->arr[0] = data;
    }

    else
    {
        q->rear++;
        q->rear = q->rear % q->capacity;
        q->arr[q->rear] = data;
    }
    // printf("done");
}

int pop(Queue q)
{
    // printf("%d\n",st->top);
    if (q->front == -1)
    {
        return -1;
    }

    else if (q->front == q->rear)
    {
        int ele = q->arr[q->front];
        q->front = -1;
        q->rear = -1;
        return ele;
    }

    else
    {
        int ele = q->arr[q->front];
        q->arr[q->front] = -111;
        q->front++;
        q->front = q->front % q->capacity;
        return ele;
    }
}

int front(Queue q)
{
    return q->front >= 0 ? q->arr[q->front] : -1;
}

void printQueue(Queue q)
{
    for (int i = 0; i <= q->capacity - 1; i++)
    {
        printf("%d ", q->arr[i]);
    }
    printf("\n");
}

void selectionSort(Process *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int mini = i;
        for (int j = i; j < n; j++)
        {
            if (arr[j].arrival < arr[mini].arrival)
            {
                mini = j;
            }
        }
        Process temp = arr[i];
        arr[i] = arr[mini];
        arr[mini] = temp;
    }
}

Process processtable[MAX_PROCESS];
int num_processes = 0;

void ReadProcessTable(char *filename)
{

    num_processes=0;

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }
    

    char name[10];
    int arrival, cpuburst;
    while (fscanf(file, "%s %d %d", name, &arrival, &cpuburst) == 3)
    {
        processtable[num_processes].name = strdup(name);
        processtable[num_processes].arrival = arrival;
        processtable[num_processes].cpuburst = cpuburst;
        processtable[num_processes].remaining_burst = cpuburst;
        processtable[num_processes].turnaround = 0;
        processtable[num_processes].wait = 0;
        num_processes++;
    }

    selectionSort(processtable, num_processes);
    fclose(file);
}

void PrintProcessTable()
{
    printf("Process Table:\n");
    for (int i = 0; i < num_processes; i++)
    {
        printf("%s %d %d\n", processtable[i].name, processtable[i].arrival, processtable[i].cpuburst);
    }
}

void FCFS()
{
    int current_time = 0;
    for (int i = 0; i < num_processes; i++)
    {
        if (processtable[i].arrival > current_time)
        {
            current_time = processtable[i].arrival;
        }
        printf("[%d - %d] P[%d] running\n", current_time, current_time + processtable[i].cpuburst, i);
        processtable[i].wait = current_time - processtable[i].arrival;
        current_time += processtable[i].cpuburst;
        processtable[i].turnaround = current_time - processtable[i].arrival;
    }
}

void PrintStatistics()
{
    double avg_turnaround = 0, avg_wait = 0;
    printf("Turnaround times: ");
    for (int i = 0; i < num_processes; i++)
    {
        printf("%s[%d] ", processtable[i].name, processtable[i].turnaround);
        avg_turnaround += processtable[i].turnaround;
        avg_wait += processtable[i].wait;
    }
    avg_turnaround /= num_processes;
    avg_wait /= num_processes;
    printf("\nWait times: ");
    for (int i = 0; i < num_processes; i++)
    {
        printf("%s[%d] ", processtable[i].name, processtable[i].wait);
    }
    printf("\nAverage turnaround time: %.2f\n", avg_turnaround);
    printf("Average wait time: %.2f\n", avg_wait);
}

void SRBF()
{
    int current_time = 0;
    int completed_processes = 0;
    int shortest_remaining_burst;
    int selected_process = -1;

    while (completed_processes < num_processes)
    {
        shortest_remaining_burst = INF;
        selected_process = -1;

        for (int i = 0; i < num_processes; i++)
        {
            if (processtable[i].arrival <= current_time && processtable[i].remaining_burst > 0)
            {
                if (processtable[i].remaining_burst < shortest_remaining_burst)
                {
                    shortest_remaining_burst = processtable[i].remaining_burst;
                    selected_process = i;
                }
            }
        }

        if (selected_process == -1)
        {
            current_time++;
        }
        else
        {
            printf("[%d - %d] %s running\n", current_time, current_time + 1, processtable[selected_process].name);
            processtable[selected_process].remaining_burst--;
            if (processtable[selected_process].remaining_burst == 0)
            {
                completed_processes++;
                processtable[selected_process].turnaround = current_time - processtable[selected_process].arrival + 1;
                processtable[selected_process].wait = processtable[selected_process].turnaround - processtable[selected_process].cpuburst;
            }
            current_time++;
        }
    }
}

void RR(int quantum)
{
    int *vis = (int *)calloc(num_processes, sizeof(int));
    Queue rq = newQueue(num_processes);
    int currentTime = 0;

    int add = 0;
    while (front(rq) == -1)
    {
        for (int i = 0; i < num_processes; i++)
        {
            if (processtable[i].arrival <= currentTime)
            {
                add = 1;
                vis[i] = 1;
                push(rq, i);
            }
        }
        if(add==0)
        {
            currentTime++;
        }
        
    }
    while (front(rq) != -1)
    {
        int selectedProcess = front(rq);
        pop(rq);
        if (processtable[selectedProcess].remaining_burst <= quantum)
        {
            printf("[%d - %d] %s running\n", currentTime, processtable[selectedProcess].remaining_burst + currentTime, processtable[selectedProcess].name);
            currentTime = currentTime + processtable[selectedProcess].remaining_burst;
            processtable[selectedProcess].remaining_burst = 0;
            processtable[selectedProcess].turnaround = currentTime - processtable[selectedProcess].arrival;
            processtable[selectedProcess].wait = processtable[selectedProcess].turnaround - processtable[selectedProcess].cpuburst;
        }
        else
        {
            printf("[%d - %d] %s running\n", currentTime, quantum + currentTime, processtable[selectedProcess].name);
            currentTime = currentTime + quantum;
            processtable[selectedProcess].remaining_burst -= quantum;
        }

        for (int i = 0; i < num_processes; i++)
        {
            if (processtable[i].arrival <= currentTime && vis[i] == 0)
            {
                vis[i] = 1;
                push(rq, i);
            }
        }

        if (processtable[selectedProcess].remaining_burst > 0)
        {
            push(rq, selectedProcess);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    ReadProcessTable(argv[1]);
    PrintProcessTable();

    int choice;
    int quantum;

    do {
        
        printf("\n-------------------------------------------------\n");
        printf("CPU Scheduling Simulation\n");
        printf("-------------------------------------------------\n");
        printf("Select the scheduling algorithm [1,2,3 or 4]:\n");
        printf("1. First Come First Served (FCFS)\n");
        printf("2. Round Robin (RR)\n");
        printf("3. SRBF\n");
        printf("4. Exit\n");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("-------------------------------------------------\n");
                printf("First Come First Served Scheduling\n");
                printf("-------------------------------------------------\n");
                FCFS();
                PrintStatistics();
                break;
            case 2:
                printf("Enter the time quantum: ");
                scanf("%d", &quantum);
                printf("-------------------------------------------------\n");
                printf("Round Robin Scheduling\n");
                printf("-------------------------------------------------\n");
                RR(quantum);
                PrintStatistics();
                break;
            case 3:
                printf("-------------------------------------------------\n");
                printf("SRBF Scheduling\n");
                printf("-------------------------------------------------\n");
                SRBF();
                PrintStatistics();
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
                break;
        }
          ReadProcessTable(argv[1]);
    } while (choice != 4);

    return 0;
}