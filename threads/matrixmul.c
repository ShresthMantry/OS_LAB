#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 3
#define K 2
#define N 3

int A[M][K] = {{1, 4}, {2, 5}, {3, 6}};
int B[K][N] = {{8, 7, 6}, {5, 4, 3}};
int C[M][N];

typedef struct {
    int i;
    int j;
} parameters;

void *multiply(void* arg) {
    parameters *params = (parameters *)arg;
    int sum = 0;
    for (int n = 0; n < K; n++) {
        sum += A[params->i][n] * B[n][params->j];
    }
    C[params->i][params->j] = sum;
    pthread_exit(0);
}

int main() {
    pthread_t threads[M][N];
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            parameters *data = (parameters *) malloc(sizeof(parameters));
            data->i = i;
            data->j = j;
            pthread_create(&threads[i][j], NULL, multiply, data);
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}