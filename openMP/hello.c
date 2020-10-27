#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void Hello(void);

int main(int argc, char *argv[])
{
    //int thread_count = strtol(argv[1], NULL, 10);
    int thread_count = omp_get_max_threads();

    printf("%d Core(s)\n", thread_count);
//#pragma omp parrallel num_threads(thread_count)
#pragma omp parallel private(thread_count)
    Hello();

    return 0;
}

void Hello(void)
{
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    printf("Hello from thread %d of %d\n", my_rank, thread_count);
}