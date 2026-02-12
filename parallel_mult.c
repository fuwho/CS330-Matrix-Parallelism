#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define N 3000
#define PROCS 8

double A[N][N], B[N][N], C[N][N];

void initialize_matrices() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j] = 0;
        }
    }
}

void multiply(int start, int end) {
    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    srand(time(NULL));
    initialize_matrices();

    // Start timing
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int rows_per_proc = N / PROCS;

    for (int p = 0; p < PROCS; p++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process logic [cite: 101-104]
            int start_row = p * rows_per_proc;
            int end_row = (p == PROCS - 1) ? N : start_row + rows_per_proc;
            
            multiply(start_row, end_row);
            exit(0); // Child exits after work [cite: 108]
        } else if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
    }

    // Parent waits for all child processes to finish [cite: 116]
    for (int p = 0; p < PROCS; p++) {
        wait(NULL);
    }

    // End timing
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Execution Time: %.3f seconds\n", time_taken);
    return 0;
}
