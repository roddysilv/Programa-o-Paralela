#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int issorted(int *myarray, int n)
{
    int issorted = 1;
    int i;
    for (i = 0; i < (n - 1); i++)
    {
        if (*(myarray + i) > *(myarray + i + 1))
        {
            issorted = 0;
        }
        break;
    }
    return issorted;
}

int main(void)
{
    int *myarray = NULL;
    int n = 100;
    int i, phase, tmp, j;
#define TOTALTRIALS 1000
    int cumulphases = 0;
    myarray = (int *)malloc(n * sizeof(int));
    srand(time(NULL));
    for (j = 0; j < TOTALTRIALS; j++)
    {
        for (i = 0; i < n; i++)
        {
            *(myarray + i) = rand() % 1000 + 100;
            printf("%5d ", *(myarray + i));
            if (((i + 1) % 10) == 0)
                printf("\n");
        }
        for (phase = 0; phase < n; phase++)
        {
            if (issorted(myarray, n) == 1)
                break;
            if ((phase & 1) == 0)
            {
                for (i = 1; i < n; i += 2)
                {
                    if (*(myarray + i - 1) > *(myarray + i))
                    {
                        tmp = *(myarray + i);
                        *(myarray + i) = *(myarray + i - 1);
                        *(myarray + i - 1) = tmp;
                    }
                }
            }
            else
            {
                for (i = 1; i < (n - 1); i += 2)
                {
                    if (*(myarray + i) > *(myarray + i + 1))
                    {
                        tmp = *(myarray + i);
                        *(myarray + i) = *(myarray + i + 1);
                        *(myarray + i + 1) = tmp;
                    }
                }
            }
        }
        printf("\n");
        for (i = 0; i < n; i++)
        {
            printf("%5d ", *(myarray + i));
            if (((i + 1) % 10) == 0)
            printf("\n");
        }
        cumulphases += (phase + 1);
    }
    printf(" Fraction saved = %5.2f\n", 1.0 - ((double)cumulphases / (double)TOTALTRIALS / (double)n));
    if (myarray)
        free(myarray);
    return 0;
}