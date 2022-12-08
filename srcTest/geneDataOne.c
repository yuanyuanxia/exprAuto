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
    unsigned long temp_input, testNumberX0;
    unsigned long sign, exponent, fraction;
    double startX0, endX0, stepX0;
    FILE *streamX0;
    char *filenameX0 = "./inputDataX0.txt";
    streamX0 = fopen(filenameX0, "w");

    if(argc != 4)
    {
        printf("Please input 3 parameters: startX0 endX0 testNumberX0\n");
        return 0;
    }
    else
    {
        startX0 = atof(argv[1]);
        endX0 = atof(argv[2]);
        testNumberX0 = atoi(argv[3]);
    }

    if(endX0 < startX0 || testNumberX0 == 0)
    {
        printf("check your input: endX0 > startX0, testNumberX0 != 0\n");
        return 1;
    }
    printf("startX0 = %f\n", startX0);
    printf("endX0 = %f\n", endX0);
    printf("testNumberX0 = %lu\n", testNumberX0);

    stepX0 = (endX0 - startX0) / testNumberX0;
    printf("stepX0 = %f\n", stepX0);

    for(size_t i = 0; i < testNumberX0; i++)
    {
        input = startX0 + i * stepX0;
        temp_input = *((unsigned long *)(&input));

        exponent = ((temp_input & EXP_BIT) >> 52);
        fraction = temp_input & TAIL_BIT;
        sign = (temp_input & SIGN_BIT) >> 63;
        
        fprintf(streamX0, "%lu\t%lu\t%lu\n", sign, exponent, fraction);
    }

    fclose(streamX0);
    return 0;
}
