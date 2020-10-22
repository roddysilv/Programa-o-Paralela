#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int nthreads, tid;

    //nthreads = omp_get_num_threads();
    nthreads = omp_get_max_threads();

    printf("Ha %d cores.\n", nthreads);

    int n = 10;
    int *l = (int *)malloc(nthreads * sizeof(int));
    int xt = 0;
    printf("Tamanho l %d\n", (int)sizeof(l));
#pragma omp parallel private(nthreads, tid)
    {

        int x = 0;
        tid = omp_get_thread_num();
        nthreads = omp_get_num_threads();

        //l = (int *)malloc(nthreads * sizeof(int));

        if (tid == 0)
        {
            printf("Com %d threads\n", nthreads);
            printf("Tamanho l %d\n", (int)sizeof(l));
        }

        for (int i = 0; i < n; i++)
        {
            x += i;
        }

        l[tid] = x;
        /*#pragma omp critical
        xt += x;*/
    }
    printf("Com %d threads\n", nthreads);
    for (int i = 0; i < nthreads; i++)
    {
        xt += l[i];
    }
    printf("x = %d\n\n", xt);
    free(l);
    return 0;
}