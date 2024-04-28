#include <stdio.h>
#include <stdbool.h>

#define MAX_RESOURCES 10
#define MAX_PROCESSES 3

int num_resources;
int resource_instances[MAX_RESOURCES];
int allocation_matrix[MAX_PROCESSES][MAX_RESOURCES];
int request_matrix[MAX_PROCESSES][MAX_RESOURCES];
bool finished[MAX_PROCESSES];
bool visited[MAX_PROCESSES] = {false};


void read_input() {
    printf("Enter the total number of distinct resources: ");
    scanf("%d", &num_resources);

    printf("Enter the number of instances for each resource (e.g., 3 5 2): ");
    for (int i = 0; i < num_resources; ++i) {
        scanf("%d", &resource_instances[i]);
    }

    printf("Enter the allocation matrix (one row per process):\n");
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        for (int j = 0; j < num_resources; ++j) {
            scanf("%d", &allocation_matrix[i][j]);
        }
    }

    printf("Enter the request matrix (one row per process):\n");
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        for (int j = 0; j < num_resources; ++j) {
            scanf("%d", &request_matrix[i][j]);
        }
    }
}

bool is_safe_state(int process, int work[]) {
    for (int i = 0; i < num_resources; ++i) {
        if (request_matrix[process][i] > work[i]) {
            return false;
        }
    }
    return true;
}

bool detect_deadlock() {
    int work[MAX_RESOURCES];
   
    for (int i = 0; i < num_resources; ++i) {
        work[i] = resource_instances[i];
    }

    int count = 0;
    while (count < MAX_PROCESSES) {
        bool found = false;
        for (int i = 0; i < MAX_PROCESSES; ++i) {
            if (!visited[i] && is_safe_state(i, work)) {
                visited[i] = true;
                for (int j = 0; j < num_resources; ++j) {
                    work[j] += allocation_matrix[i][j];
                }
                found = true;
                count++;
            }
        }
        if (!found) {
            break;
        }
    }

    return count != MAX_PROCESSES;
}

int main() {
    read_input();

    if (detect_deadlock()) {
        printf("Deadlock exists.\n");
        printf("Smallest set of processes whose termination ends the deadlock:\n");
        for (int i = 0; i < MAX_PROCESSES; ++i) {
            if (!visited[i]) {
                printf("P%d\n", i);
            }
        }
    } else {
        printf("No deadlock.\n");
    }

    return 0;
}