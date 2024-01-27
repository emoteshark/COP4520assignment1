#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>

#define LAST 100000000
#define NTHREADS 8

struct _primetest { int value; }  array[LAST+1] = { 0 };
//where value is:
// 0 not tested
// 1 prime
// 2 not prime


void intprimetest(int n)
{
    int done = 0;
    double x = sqrt((double) n);
    int limit = (int)(x);

    //printf("limit for %d is %d\n", n, limit);
    for(int j=2;j<=limit;j++)
    {
        //printf("testing %d mod %d = %d\n", n, j, (n%j));
        if (array[j].value == 2)
            continue;

        if ((n % j) == 0)
        {
            //printf("%d is NOT prime\n", n);
            array[n].value = 2;
            done = 1;
            break;
        }
    }
    if (!done)
    {
        //printf("%d is prime\n", n);
        array[n].value = 1;
    }
}


void *fillarray(void *n)
{
    int start = (int) n;

    for(int i=start;i<=LAST;i+=NTHREADS)
    {
        int done = 0;

        if ((i%2)==0 || (i%3)==0)
        {
            array[i].value = 2;
            continue;
        }

        for(int j=5; j*j<=i; j+=6)
        {
            // if (array[j].value == 2) continue;
            // if (array[j+2].value == 2) continue;

            // printf("testing %d mod %d = %d\n", i, j, (i%j));
            if ((i % j) == 0 || (i%(j+2)) == 0)
            {
                // printf("%d is NOT prime\n", i);
                array[i].value = 2;
                done = 1;
                break;
            }
        }
        if (!done)
        {
            // printf("%d is prime\n", i);
            array[i].value = 1;
        }
        // fflush(stdout);
    }

    return NULL;
}

int main()
{
    //clock_t t1, t2;
    struct timeval tv1, tv2;

    pthread_t tarray[NTHREADS] = { 0 };

    //t1 = clock();
    gettimeofday(&tv1, NULL);

#ifdef DO_EARLY_PRIMES
    for(int i=2;i<10000;i++)
    {
        intprimetest(i);
    }
#else
    array[2].value = 1;
    array[3].value = 1;
    array[4].value = 2;
    array[5].value = 1;
#endif

    for(int i=0;i<NTHREADS;i++)
    {
#ifdef DO_EARLY_PRIMES
        if (pthread_create(&tarray[i], NULL, fillarray, (void*)10001+i))
#else
        if (pthread_create(&tarray[i], NULL, fillarray, (void*)6+i))
#endif
        {
            printf("create failed %d\n", errno);
            exit(-1);
        }
    }

    for(int i=0;i<NTHREADS;i++)
    {
        void *res;
        pthread_join(tarray[i], &res);
        free(res);
    }
    
    //t2 = clock();
    gettimeofday(&tv2, NULL);


    int total = 0;
    unsigned long long sum = 0;
    for(int i=2; i<LAST; i++)
    {
        if (array[i].value == 1)
        {
            sum += i;
            total++;
        }
    }

#if 0
    int count = 0;
    for(int i=LAST-1;i>2;i--)
    {
        if (array[i].value == 1)
        {
            printf("%d\n", i);
            count++;
        }
        if (count>9)
            break;
    }
#endif

    FILE *f = fopen("primes.txt", "w");

    fprintf(f, "%ld ", tv2.tv_sec - tv1.tv_sec);
    fprintf(f, "%d ", total);
    fprintf(f, "%llu ", sum);

    for(int i=99999787;i<LAST;i++)
        if (array[i].value == 1)
            fprintf(f, "%d ", i);

    //printf("sum = %llu\n", sum);
    //printf("total = %d\n", total);
}
