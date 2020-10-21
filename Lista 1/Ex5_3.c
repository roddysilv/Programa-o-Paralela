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

void Odd_even_sort(
    int a[] /* in/out */,
    int n /* in     */)
{
    int phase, i, temp;

    for (phase = 0; phase < n; phase++)
    {
        if (phase % 2 == 0)
        { /* Even phase */
            for (i = 1; i < n; i += 2)
                if (a[i - 1] > a[i])
                {
                    temp = a[i];
                    a[i] = a[i - 1];
                    a[i - 1] = temp;
                }
        }
        else
        { /* Odd phase */
            for (i = 1; i < n - 1; i += 2)
                if (a[i] > a[i + 1])
                {
                    temp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = temp;
                }
        }
        if (isSorted(a, n) == true)
        {
            break;
        }
    } /* Odd_even_sort */
}

void oddEvenModificado(int *l, int n)
{
    int p = 0, f1 = 0, f2 = 0;
    bool sorted = isSorted(l, n);
    while (sorted != true)
    {
        p++;
        sorted = true;
        for (int i = 1; i < n - 1; i += 2)
        {
            if (l[i] > l[i + 1])
            {
                f1++;
                int aux;
                aux = l[i];
                l[i] = l[i + 1];
                l[i + 1] = aux;
                sorted = false;
            }
        }
        for (int i = 0; i < n - 1; i += 2)
        {
            if (l[i] > l[i + 1])
            {
                f2++;
                int aux;
                aux = l[i];
                l[i] = l[i + 1];
                l[i + 1] = aux;
                sorted = false;
            }
        }
    }
    //printf("Fases: %d\n\n", p);
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

    //oddEvenModificado(local_v, local_n);
    Odd_even_sort(local_v, local_n);

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