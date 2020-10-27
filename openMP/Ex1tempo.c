#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void Trap(double a, double b, int n, double *global_result_p);

double f(double x);

int main(int argc, char *argv[])
{
    clock_t start, end;
    double cpu_time_used;

    double global_result;
    double a = 0;
    double b = 100;
    int n = 2000000000;
    int thread_count;

    //thread_count = strtol(argv[1], NULL, 10);
    thread_count = omp_get_max_threads();

    printf("%d Core(s)\n", thread_count);

    //printf("Enter a, b, and n\n");
    //scanf("%lf %lf %d", &a, &b, &n);

    for (int j = 1; j <= 32; j = j * 2)
    {
        cpu_time_used = 0;
        thread_count = j;

        for (int i = 0; i < 5; i++)
        {
            global_result = 0.0;

            start = time(NULL);
#pragma omp parallel num_threads(thread_count)
            Trap(a, b, n, &global_result);

            end = time(NULL);

            cpu_time_used += end - start;
        }
        cpu_time_used = cpu_time_used / 5;
        printf("%d Core(s) Tempo Médio: %f s\n", thread_count, cpu_time_used);
    }

    //printf("With n = %d trapezoids, our estimate\n", n);
    //printf("of the integral from %f to %f = %.14e\n", a, b, global_result);

    return 0;
}

void Trap(double a, double b, int n, double *global_result_p)
{
    double h, x, my_result;
    double local_a, local_b;
    int i, local_n;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    //printf("Rank: %d\n",my_rank);

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

#pragma omp critical
    *global_result_p += my_result;
}

double f(double x)
{
    return x * x;
}