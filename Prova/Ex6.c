#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long int monteCarlo(long int lancamentos, int my_rank)
{
    double x, y;
    long int acertos = 0;
    srand(time(0) + my_rank);
    for (int i = 0; i < lancamentos; i++)
    {
        x = -1 + ((float)rand() / (float)(RAND_MAX)) * (1 - (-1));
        y = -1 + ((float)rand() / (float)(RAND_MAX)) * (1 - (-1));
        if ((x * x + y * y) <= 1)
        {
            acertos++;
        }
    }
    return acertos;
}

int main(int argc, char *argv[])
{
    int my_rank;
    int num_procs;
    long int lancamentos = 0;
    double pi = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    long int lancamentos_local = 0;
    long int acertos_local = 0;
    long int acertos_total = 0;

    if (my_rank == 0)
    {
        printf("Entre com a quantidade de lancamentos:");
        scanf("%ld", &lancamentos);
    }

    MPI_Bcast(&lancamentos, 1, MPI_INT, 0, MPI_COMM_WORLD);

    lancamentos_local = lancamentos / num_procs;

    acertos_local = monteCarlo(lancamentos_local, my_rank);

    MPI_Reduce(&acertos_local, &acertos_total, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        pi = 4 * (double)acertos_total / ((double)lancamentos);
        printf("O valor de pi eh de aproximadamente: %f\n", pi);
    }

    MPI_Finalize();

    return 0;
}