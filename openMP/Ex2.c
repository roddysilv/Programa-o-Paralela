#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Trap(double a, double b, int n, double *global_result_p);

double f(double x);

int main(int argc, char *argv[])
{
    double a = 0;
    double b = 100;
    double x = 0;
    int n = 500000000;
    int thread_count;

    //thread_count = strtol(argv[1], NULL, 10); // Dá segmentation fault(core dumped) no meu pc
    //thread_count = omp_get_max_threads();
    
    thread_count = 32;

    printf("%d Core(s)\n", thread_count);

    double *global_result_vector = (double *)malloc(thread_count * sizeof(double));

    //printf("Enter a, b, and n\n");
    //scanf("%lf %lf %d", &a, &b, &n);

#pragma omp parallel num_threads(thread_count)
    Trap(a, b, n, global_result_vector);

    for (int i = 0; i < thread_count; i++)
    {
        x += global_result_vector[i];
    }
    free(global_result_vector);

    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.14e\n", a, b, x);
    return 0;
}

void Trap(double a, double b, int n, double *global_result_p)
{
    double h, x, my_result;
    double local_a, local_b;
    int i, local_n;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    h = (b - a) / n;
    local_n = n / thread_count;
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    my_result = (f(local_a) + f(local_b)) / 2.0;

    for (i = 0; i <= local_n - 1; i++)
    {
        x = local_a + i * h;
        my_result += f(x);
    }
    my_result = my_result * h;

    global_result_p[my_rank] = my_result;
}

double f(double x)
{
    return x * x;
}