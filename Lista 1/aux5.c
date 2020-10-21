#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

bool isSorted(int *l, int n)
{
    for (int i = 1; i < n; i++)
    {
        if (l[i - 1] > l[i])
        {
            return false;
        }
    }
    return true;
}
void troca(int A[], int n, int i)
{
    j = i + 1;
    if (A[i] > A[j])
    {
        temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}

oddEven(double A[], int n, int my_rank, int proc)
{
    for (int i = 0; i < proc; i++)
    {
        if (i % 2 == 1)
        {
            if (my_rank % 2 == 1)
            {

                if (my_rank < proc - 1)
                //MPI_SEND
            }
            else if (my_rank > 0)
            //MPI_SEND
        }
        if (i % 2 == 0)
        {
            if (my_rank % 2 == 0)
            {
                if (my_rank < proc - 1)
                //MPI
            }
            else
            //MPI
        }
    }
}

int main(int argc, char *argv[])
{
    srand(time(0));

    int n = 80000;
    int my_rank, proc;
    int v[n];
    int vr[n];

    int min = 1;
    int max = 100;

    for (int i = 0; i < n; i++)
    {
        v[i] = min + (rand() / (float)(RAND_MAX)) * (max - min);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int local_n = n / proc;
    int local_v[local_n];

    MPI_Scatter(v, local_n, MPI_INT, local_v, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    oddEven(local_v, local_n, my_rank, proc);

    MPI_Gather(local_v, local_n, MPI_INT, vr, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        //oddEvenModificado(vr, n);
        Odd_even_sort(vr, n);
        /*for (int i = 0; i < n; i++)
        {
            printf("%d ", vr[i]);
        }*/
    }

    MPI_Finalize();

    return 0;
}