#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int nthreads, tid;
    int thread_count;

    int n = 10;
    int l[10];
    int xt = 0;
    int x = 0;

#pragma omp parallel private(nthreads, tid)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Com %d threads", nthreads);
        }
        for (int i = 0; i < n; i++)
        {
            x += i;
        }
#pragma omp critical
        xt += x;
    }

printf("\nx = %d\n\n", x);
return 0;
}