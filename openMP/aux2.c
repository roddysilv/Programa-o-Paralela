#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int thread_count;

    int n = 10;
    int l[10];
    int xt = 0;
    int x = 0;

#pragma omp parallel num_threads(thread_count)
    {
        if(omp_get_thread_num()==0){
            printf("Com %d threads",omp_get_num_threads());
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