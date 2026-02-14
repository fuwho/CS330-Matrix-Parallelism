#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define N 3000
#define PROCS 8

// Matrix size (can be varied)
// Number of child processes
double A [N] [N], B[N] [N], C[N][N];

void initialize_matrices() {
    for (int i=0; i < N; i++)
        for (int j=0; j<N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j]=0 ;
        }
}

void multiply (int start, int end) {
    for (int i= start; i < end; i++)
        for (int j=0; j<N; j++)
            for (int k=0; k<N; k++)
                C[i][j]+=A[i][k]*B[k][j] ;
}

int main() {
    srand(time(NULL));
    initialize_matrices();
    clock_t start_time = clock();
    int rows_per_proc = N / PROCS;
    for (int p=0; p < PROCS; p++) {
        pid_t pid = fork();
        if (pid == 0) {
            int start = p * rows_per_proc;
            int end = (p == PROCS - 1)? N: start + rows_per_proc;
            multiply (start, end);
            exit(0);
        }
    }

    for (int p=0; p < PROCS; p++)
        wait (NULL);

    clock_t end_time = clock();
    double time_taken = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("ExecutionTime: %.7f seconds\n", time_taken);
    return 0;
}
