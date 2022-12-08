/***
 *  Generate Float Numbers with condition
 *  
 *  Give the interval and the amount of data taken within the interval, and return the corresponding data set
 *  like input [A,B] N
 *  return N data between A and B
***/

#include <fcntl.h>
#include <sched.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "common.h"

int main(int argc, char *argv[])
{
    double input;
    unsigned long temp_input, testNumberX0, testNumberX1, testNumberX2;
    double startX0, endX0, startX1, endX1, startX2, endX2, stepX0, stepX1, stepX2;
    FILE *streamX0, *streamX1, *streamX2;
    char *filenameX0 = "./inputDataX0.txt";
    char *filenameX1 = "./inputDataX1.txt";
    char *filenameX2 = "./inputDataX2.txt";
    streamX0 = fopen(filenameX0, "w");
    streamX1 = fopen(filenameX1, "w");
    streamX2 = fopen(filenameX2, "w");

    if(argc != 10)
    {
        printf("Please input 9 parameters: startX0 endX0 startX1 endX1 startX2 endX2 testNumberX0 testNumberX1 testNumberX2\n");
        return 0;
    }
    else
    {
        startX0 = atof(argv[1]);
        endX0 = atof(argv[2]);
        startX1 = atof(argv[3]);
        endX1 = atof(argv[4]);
        startX2 = atof(argv[5]);
        endX2 = atof(argv[6]);
        testNumberX0 = atoi(argv[7]);
        testNumberX1 = atoi(argv[8]);
        testNumberX2 = atoi(argv[9]);
    }

    if(endX0 < startX0 || testNumberX0 == 0 || endX1 < startX1 || testNumberX1 == 0 || endX2 < startX2 || testNumberX2 == 0)
    {
        printf("check your input: end > start, testNumber != 0\n");
        return 1;
    }
    stepX0 = (endX0 - startX0) / testNumberX0;
    stepX1 = (endX1 - startX1) / testNumberX1;
    stepX2 = (endX2 - startX2) / testNumberX2;

    printf("startX0 = %f\n", startX0);
    printf("endX0 = %f\n", endX0);
    printf("testNumberX0 = %lu\n", testNumberX0);
    printf("stepX0 = %f\n", stepX0);
    printf("startX1 = %f\n", startX1);
    printf("endX1 = %f\n", endX1);
    printf("testNumberX1 = %lu\n", testNumberX1);
    printf("stepX1 = %f\n", stepX1);
    printf("startX2 = %f\n", startX2);
    printf("endX2 = %f\n", endX2);
    printf("testNumberX2 = %lu\n", testNumberX2);
    printf("stepX2 = %f\n", stepX2);

    // for dimension X0
    for(size_t i = 0; i <= testNumberX0; i++)
    {
        input = startX0 + i * stepX0;
        temp_input = *((unsigned long *)(&input));
        fprintf(streamX0, "%lx\n", temp_input);
    }

    // for dimension X1
    for(size_t i = 0; i <= testNumberX1; i++)
    {
        input = startX1 + i * stepX1;
        temp_input = *((unsigned long *)(&input));
        fprintf(streamX1, "%lx\n", temp_input);
    }

    // for dimension X2
    for(size_t i = 0; i <= testNumberX2; i++)
    {
        input = startX2 + i * stepX2;
        temp_input = *((unsigned long *)(&input));
        fprintf(streamX2, "%lx\n", temp_input);
    }

    fclose(streamX0);
    fclose(streamX1);
    fclose(streamX2);
    return 0;
}
