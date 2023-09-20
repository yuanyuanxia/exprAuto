#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <mathimf.h>
#include <math.h>
#include <stdint.h>

#include "common.h"

#ifndef FUNCNAME
#define FUNCNAME i6
#endif
#ifndef RUNTIME
#define RUNTIME 10000
#endif

double FUNCNAME(double, double);
/* copy: copy 'from' into 'to'; assume to is big enough */
void copy(char to[], char from[]);

// for performance test
extern inline __attribute__((always_inline)) unsigned long rdtscp()
{
    unsigned long a, d, c;

    __asm__ volatile("rdtscp" : "=a"(a), "=d"(d), "=c"(c));

    return (a | (d << 32));
}

int main(int argc, char *argv[])
{
    int i;
    unsigned long times[RUNTIME];
    unsigned long sum;
    uint64_t tic, toc;
    double inputdataX0[RUNTIME], inputdataX1[RUNTIME];
    double result[RUNTIME], sumResult = 0, test = 1;
    double data_x0Start = 0, data_x0End = 1, data_x1Start = 0, data_x1End = 1;
    FILE *stream_time;
    char outputFile[64] = "FUNCNAME_time.txt";  // the name of the outputFile.

    srand((unsigned)time(NULL));
    if(argc == 2)
    {
        copy(outputFile, argv[1]);
    }
    if(argc == 6)
    {
        copy(outputFile, argv[1]);
        data_x0Start = atof(argv[2]);
        data_x0End = atof(argv[3]);
        data_x1Start = atof(argv[4]);
        data_x1End = atof(argv[5]);
    }
    stream_time = fopen(outputFile, "w");
    if(stream_time == (FILE *)0)
    {
        printf("open file error!\n");
        exit(1);
    }

    // printf("----x0Starting generating %d test data----\n", RUNTIME);
    for(i = 0; i < RUNTIME; i++)
    {
        inputdataX0[i] = (double)rand() / ((double)RAND_MAX + 1) * ((data_x0End) - (data_x0Start)) + (data_x0Start);
        inputdataX1[i] = (double)rand() / ((double)RAND_MAX + 1) * ((data_x1End) - (data_x1Start)) + (data_x1Start);
    }
    // printf("----End generating %d test data----\n", RUNTIME);

    // printf("----x0Starting performance test----\n");
    for(i = 0; i < 100; i++)
    {
        FUNCNAME(inputdataX0[i], inputdataX1[i]);
    }
    for(i = 0; i < RUNTIME; i++)
    {
        tic = rdtscp();
        result[i] = FUNCNAME(inputdataX0[i], inputdataX1[i]);
        toc = rdtscp();
        // printf("the %d result is %f\n", i, result[i]);
        result[i] = result[i] + test;
        times[i] = toc - tic;
        // printf("time is %llu clock cycles\n", times[i]);
    }

    // printf("----End performance test----\n");

    sum = 0;
    for(i = 0; i < RUNTIME; i++)
    {
        fprintf(stream_time, "%lu\n", times[i]);
        sum = sum + times[i];
        sumResult = sumResult + result[i];
    }
    sum = sum / RUNTIME;
    // printf("sumResult is %f\n", sumResult);
    // printf("the performance result to be clean: %lu\n", sum);
    if(fclose(stream_time) == EOF)
    {
        printf("close file error!\n");
        exit(1);
    }
}
