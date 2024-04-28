#include <stdio.h>
#include <stdbool.h>

#define MAX_RESOURCES 10
#define MAX_PROCESSES 5

int num_resources;
int resource_instances[MAX_RESOURCES];
int allocation_matrix[MAX_PROCESSES][MAX_RESOURCES];
int request_matrix[MAX_PROCESSES][MAX_RESOURCES];
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

bool dfs(int process) {
    visited[process] = true;
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        if (!visited[i]) {
            for (int j = 0; j < num_resources; ++j) {
                if (request_matrix[process][j] > 0 && allocation_matrix[i][j] > 0) {
                    if (dfs(i)) {
                        return true;
                    }
                }
            }
        } else if (i != process) {
            for (int j = 0; j < num_resources; ++j) {
                if (request_matrix[process][j] > 0 && allocation_matrix[i][j] > 0) {
                    return true;
                }
            }
        }
    }
    visited[process] = false;
    return false;
}

bool detect_deadlock() {
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        if (dfs(i)) {
            return true;
        }
    }
    return false;
}

int main() {
    read_input();

    if (detect_deadlock()) {
        printf("\nDeadlock exists.\n");
        printf("Smallest set of processes whose termination ends the deadlock:\n");
        for (int i = 0; i < MAX_PROCESSES; ++i) {
            if (visited[i]) {
                printf("P%d\n", i);
            }
        }
    } else {
        printf("No deadlock.\n");
    }

    return 0;
}