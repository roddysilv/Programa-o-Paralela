/* Parallel dot product */
#include <mpi.h>
#include <stdio.h>

const int N = 40;

int dotProduct(int *x, int *y, int n)
{
  int i;
  int prod = 0;
  for (i = 0; i < n; i++)
  {
    prod += x[i] * y[i];
  }
  return prod;
}

int main(int argc, char *argv[])
{
  int my_rank, comm_sz;
  int v[N];

  for (int i = 0; i < N; i++)
  {
    v[i] = i;
  }

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int local_N = N / comm_sz; //assuming N is totally divisible by comm_sz
  int local_v[local_N];

  MPI_Scatter(v, local_N, MPI_INTEGER, local_v, local_N, MPI_INTEGER, 0, MPI_COMM_WORLD);

  //if (my_rank == 0)
  //{
    printf("P = %d\n", my_rank);
    for (int i = 0; i < local_N; i++)
    {
      printf("%d ", local_v[i]);
    }
    printf("\n");
  //}

  MPI_Finalize();

  return 0;
}