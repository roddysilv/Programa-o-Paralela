#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long int prodEscalar(long int *v1, long int *v2, long int n)
{
  long int r = 0;
  for (long int i = 0; i < n; i++)
  {
    r += v1[i] * v2[i];
  }
  return r;
}

void prodVetorial(long int *v, long int *vr, long int escalar, long int n)
{
  for (long int i = 0; i < n; i++)
  {
    vr[i] = v[i] * escalar;
  }
}

int main(int argc, char *argv[])
{

  srand(time(0));

  long int n;
  long int r;
  int my_rank;
  int num_procs;
  long int escalar;

  int v1[n], v2[n], vr[n];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank == 0)
  {
    printf("Entre com o tamanho dos vetors e o escalar a\n");
    scanf("%ld %ld", &n, &escalar);

    for (int i = 0; i < n; i++)
    {
      v1[i] = rand();
      v2[i] = rand();
    }

    printf("v1: ");
    for (int i = 0; i < n; i++)
    {
      printf("%d ", v1[i]);
    }
    printf("\n");

    printf("v2: ");
    for (int i = 0; i < n; i++)
    {
      printf("%d ", v2[i]);
    }

    printf("\nEscalar: %ld", escalar);
    printf("\n\n");
  }

  long int local_n = n / num_procs;
  long int local_v1[local_n];
  long int local_v2[local_n];
  long int local_vr[local_n];

  MPI_Scatter(v1, local_n, MPI_INTEGER, local_v1, local_n, MPI_INTEGER, 0, MPI_COMM_WORLD);
  MPI_Scatter(v2, local_n, MPI_INTEGER, local_v2, local_n, MPI_INTEGER, 0, MPI_COMM_WORLD);

  long int local_r = prodEscalar(local_v1, local_v2, local_n);
  prodVetorial(local_v1, local_vr, escalar, local_n);

  MPI_Reduce(&local_r, &r, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Gather(local_vr, local_n, MPI_INT, vr, local_n, MPI_INT, 0, MPI_COMM_WORLD);

  if (my_rank == 0)
  {
    printf("O produto escalar de v1 por v2 eh: %ld", r);
    printf("\n");

    printf("O produto vetorial de v1 por %ld eh: ", escalar);
    for (int i = 0; i < n; i++)
    {
      printf("%d ", vr[i]);
    }
  }

  MPI_Finalize();

  return 0;
}