#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void oddEven(int *l, int n, int proc)
{
    #pragma omp parallel
    int temp;
    int exch = 1, start = 0, i;
    while (1)
    {
        if (exch == 0 && start == 0)
        {
            break;
        }
        exch--;
        for (i = start; i < n; i += 2)
        {
            if (l[i] > l[i + 1])
            {
                temp = l[i];
                l[i] = l[i + 1];
                l[i + 1] = temp;
                exch = proc;
            }
        }
        if (start == 0)
        {
            start = 1;
        }
        else
        {
            start = 0;
        }
    }
}

int main(int argc, char *argv[])
{

    srand(time(0));

    int n = 200;
    int my_rank, procc;
    int v1[n];
    int vr[n];

    int min = 1;
    int max = 100;

    //input(my_rank, n, escalar, min, max);

    for (int i = 0; i < n; i++)
    {
        v1[i] = min + (rand() / (float)(RAND_MAX)) * (max - min);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procc);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int local_n = n / procc;
    int local_v1[local_n];

    MPI_Scatter(v1, local_n, MPI_INT, local_v1, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    oddEven(local_v1, local_n, procc);

    MPI_Gather(local_v1, local_n, MPI_INT, vr, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        for (int i = 0; i < n; i++)
        {
            printf("%d ", vr[i]);
        }
    }

    MPI_Finalize();

    return 0;
}