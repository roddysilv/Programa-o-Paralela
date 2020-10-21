#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int f(int *x, int *y, int n)
{
    int i;
    int prod = 0;
    for (i = 0; i < n; i++)
    {
        prod += x[i] * y[i];
    }
    return prod;
}

void f2(int *v, int *r, int a, int n)
{
    for (int i = 0; i < 10; i++)
    {
        r[i] = v[i] * a;
    }
}

void input(int my_rank, int *lv1, int *lv2, int ln)
{
    if (my_rank == 0)
    {
        srand(time(0));
        int n, a;
        printf("Tamanho e escalar: ");
        scanf("%d %d", &n, &a);

        int *temp1 = (int *)malloc(n * sizeof(int));
        int *temp2 = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
        {
            temp1[i] = 0 + (rand() / (float)(RAND_MAX)) * (10 - 0);
            temp2[i] = 0 + (rand() / (float)(RAND_MAX)) * (10 - 0);
        }

        MPI_Scatter(temp1, ln, MPI_INT, lv1, ln, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(temp2, ln, MPI_INT, lv2, ln, MPI_INT, 0, MPI_COMM_WORLD);

        free(temp1);
        free(temp2);
    }
}

int main(int argc, char *argv[])
{
    int my_rank, procc;
    int vr[10];
    int r;
    int n, a;
    int min = 0, max = 10;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procc);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0)
    {
        srand(time(0));
        printf("Os vetores serao gerado aleatoriamente entre %d e %d\n", min, max);
        printf("Entre com o tamanho do vetor e um escalar: ");
        scanf("%d %d", &n, &a);
    }

    int ln = n / procc;
    int lv1[ln];
    int lv2[ln];
    int lvr[ln];
    int lr;

    if (my_rank == 0)
    {
        int *temp1 = (int *)malloc(n * sizeof(int));
        int *temp2 = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
        {
            temp1[i] = min + (rand() / (float)(RAND_MAX)) * (max - min);
            temp2[i] = min + (rand() / (float)(RAND_MAX)) * (max - min);
        }
        printf("v1: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", temp1[i]);
        }
        printf("\nv2: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", temp2[i]);
        }
        printf("\n\n");
        printf("%d ", ln);
        MPI_Scatter(temp1, ln, MPI_INT, lv1, ln, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(temp2, ln, MPI_INT, lv2, ln, MPI_INT, 0, MPI_COMM_WORLD);

        printf("teste");

        free(temp1);
        free(temp2);
    }

    lr = f(lv1, lv2, ln);
    f2(lv1, lvr, a, ln);

    MPI_Reduce(&lr, &r, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Gather(lvr, ln, MPI_INT, vr, ln, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        printf("O produto escalar entre v1 e v2 eh: %d\n", r);
        printf("O produto vetorial entre v1 e %d eh: ", a);
        for (int i = 0; i < n; i++)
        {
            printf("%d ", vr[i]);
        }
    }
    MPI_Finalize();
    printf("\n\n");

    return 0;
}