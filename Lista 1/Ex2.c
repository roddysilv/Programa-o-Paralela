#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double prodEcalar(int *x, int *y, int n)
{
    int i;
    double prod = 0;
    for (i = 0; i < n; i++)
    {
        prod += x[i] * y[i];
    }
    return prod;
}

void prodVetorial(int *v, int *vr, int escalar, int n)
{
    for (int i = 0; i < n; i++)
    {
        vr[i] = v[i] * escalar;
    }
}

void input(int n, int escalar, int min, int max)
{
    printf("Os vetores serao gerado aleatoriamente entre %d e %d\n", min, max);
    printf("Entre com o tamanho do vetor e um escalar: ");
    scanf("%d %d", &n, &escalar);
}

int main(int argc, char *argv[])
{

    srand(time(0));

    int n = 20000;
    int escalar = 1;
    int my_rank, procc;
    int v1[n], v2[n], vr[n];
    double r = 0;

    int min = 1;
    int max = 100;

    //input(n, escalar, min, max);

    for (int i = 0; i < n; i++)
    {
        v1[i] = min + (rand() / (float)(RAND_MAX)) * (max - min);
        v2[i] = min + (rand() / (float)(RAND_MAX)) * (max - min);
        //v1[i] = 1;
        //v2[i] = 1;
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procc);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int local_n = n / procc;
    int local_v1[local_n];
    int local_v2[local_n];
    int local_vr[local_n];
    double local_r;

    MPI_Scatter(v1, local_n, MPI_INT, local_v1, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(v2, local_n, MPI_INT, local_v2, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 100000; i++)
    {
        local_r = prodEcalar(local_v1, local_v2, local_n);
        prodVetorial(local_v1, local_vr, escalar, local_n);
    }

    MPI_Reduce(&local_r, &r, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Gather(local_vr, local_n, MPI_INT, vr, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        printf("%f\n", r);
        printf("%d\n\n", n);
        /*for (int i = 0; i < n; i++)
        {
            printf("%d ", v1[i]);
        }*/
    }

    MPI_Finalize();

    return 0;
}