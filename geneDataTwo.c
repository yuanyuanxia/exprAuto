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
    unsigned long temp_input, testNumberX0, testNumberX1;
    double startX0, endX0, startX1, endX1, stepX0, stepX1;
    FILE *streamX0, *streamX1;
    char *filenameX0 = "./inputDataX0.txt";
    char *filenameX1 = "./inputDataX1.txt";
    streamX0 = fopen(filenameX0, "w");
    streamX1 = fopen(filenameX1, "w");

    if(argc != 7)
    {
        printf("Please input 6 parameters: startX0 endX0 startX1 endX1 testNumberX0 testNumberX1\n");
        return 0;
    }
    else
    {
        startX0 = atof(argv[1]);
        endX0 = atof(argv[2]);
        startX1 = atof(argv[3]);
        endX1 = atof(argv[4]);
        testNumberX0 = atoi(argv[5]);
        testNumberX1 = atoi(argv[6]);
    }

    if(endX0 < startX0 || testNumberX0 == 0 || endX1 < startX1 || testNumberX1 == 0)
    {
        printf("check your input: endX0 > startX0, testNumberX0 != 0\n");
        return 1;
    }
    stepX0 = (endX0 - startX0) / testNumberX0;
    stepX1 = (endX1 - startX1) / testNumberX1;

    printf("startX0 = %f\n", startX0);
    printf("endX0 = %f\n", endX0);
    printf("testNumberX0 = %lu\n", testNumberX0);
    printf("stepX0 = %f\n", stepX0);
    printf("startX1 = %f\n", startX1);
    printf("endX1 = %f\n", endX1);
    printf("testNumberX1 = %lu\n", testNumberX1);
    printf("stepX1 = %f\n", stepX1);

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

    fclose(streamX0);
    fclose(streamX1);
    return 0;
}
