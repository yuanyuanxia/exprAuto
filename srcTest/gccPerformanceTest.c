#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <mathimf.h>
#include <math.h>
#include <stdint.h>

#include "common.h"

#ifndef FUNCNAME
#define FUNCNAME sin
#endif
#ifndef RUNTIME
#define RUNTIME 10000
#endif

double FUNCNAME(double);
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
    uint64_t start, end;
    double inputdata[RUNTIME];
    double result[RUNTIME], sumResult = 0, test = 1;
    double data_start = 0, data_end = 1;
    FILE *stream_time;
    char outputFile[64] = "FUNCNAME_time.txt";  // the name of the outputFile.

    srand((unsigned)time(NULL));
    if(argc == 2)
    {
        copy(outputFile, argv[1]);
        // printf("Performance test function: %s\n", outputFile);
        // printf("outputFile is %s.\ntest from %f, end at %f by default\n", outputFile, data_start, data_end);
    }
    if(argc == 4)
    {
        copy(outputFile, argv[1]);
        data_start = atof(argv[2]);
        data_end = atof(argv[3]);
        // printf("Performance test function: %s\n", outputFile);
        // printf("outputFile is %s.\ntest from %f, end at %f\n", outputFile, data_start, data_end);
    }
    stream_time = fopen(outputFile, "w");
    if(stream_time == (FILE *)0)
    {
        printf("open file error!\n");
        exit(1);
    }

    // printf("----Starting generating %d test data----\n", RUNTIME);
    for(i = 0; i < RUNTIME; i++)
    {
        inputdata[i] = (double)rand() / ((double)RAND_MAX + 1) * ((data_end) - (data_start)) + (data_start);
    }
    // printf("----End generating %d test data----\n", RUNTIME);

    // printf("----Starting performance test----\n");
    for(i = 0; i < 100; i++)
    {
        FUNCNAME(inputdata[i]);
    }
    for(i = 0; i < RUNTIME; i++)
    {
        start = rdtscp();
        result[i] = FUNCNAME(inputdata[i]);
        end = rdtscp();
        // printf("the %d result is %f\n", i, result[i]);
        result[i] = result[i] + test;
        times[i] = end - start;
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
    printf("sumResult is %f\n", sumResult);
    // printf("the performance result to be clean: %lu\n", sum);
    if(fclose(stream_time) == EOF)
    {
        printf("close file error!\n");
        exit(1);
    }
}
