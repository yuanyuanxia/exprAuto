/***
 *  Generate Float Numbers with condition
 *
 *  Give the interval and the amount of data taken within the interval, and return the corresponding data set
 *  like input [A,B] N
 *  return N data between A and B
 ***/

#include <stdio.h>
#include <time.h>

#include "common.h"

#define NUM 524288 // 2^19

int main(int argc, char *argv[])
{
    // exception handling
    srand((unsigned)time(NULL));
    if(argc < 3)
    {
        printf("please enter some arguments.\n");
        exit(EXIT_FAILURE);
    }
    else if(argc % 2 != 1)
    {
        printf("please enter even number of arguments.\n");
        exit(EXIT_FAILURE);
    }
    // basic initail
    int i, j;
    int dimension = (argc - 1) / 2;
    double *intervals;
    intervals = (double *)calloc(dimension * 2, sizeof(double));
    for(i = 0; i < dimension; i++)
    {
        intervals[2 * i + 0] = atof(argv[1 + 2 * i + 0]);
        intervals[2 * i + 1] = atof(argv[1 + 2 * i + 1]);
    }
    // FILE initail
    FILE *file;
    char *filename = "./inputData.txt";
    if(!(file = fopen(filename, "w")))
    {
        printf("Couldn't open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // FILE **files;
    // int numOfFiles = atoi(argv[argc - 1]);
    // printf("Number of files: %d\n", numOfFiles);
    // files = (FILE **)calloc(numOfFiles, sizeof(FILE *));
    // char *filenamePrefix = "./inputDataX";
    // char *filenameSuffix = ".txt";
    // char *filenameTmp;
    // filenameTmp = (char *)calloc(strlen(filenamePrefix) + strlen(filenameSuffix) + 8, sizeof(char));
    // for(i = 0; i < numOfFiles; i++)
    // {
    //     sprintf(filenameTmp, "%s%d%s", filenamePrefix, i, filenameSuffix);
    //     printf("files[%d]: %s\n", i, filenameTmp);
    //     if(!(files[i] = fopen(filenameTmp, "w")))
    //     {
    //         printf("Couldn't open file: %s\n", filenameTmp);
    //         exit(EXIT_FAILURE);
    //     }
    // }
    
    // main body
    double *temps = (double *)calloc(dimension, sizeof(double));
    for(i = 0; i < NUM; i++)
    {
        for(j = 0; j < dimension; j++)
        {
            temps[j] = intervals[2 * j + 0] + (intervals[2 * j + 1] - intervals[2 * j + 0]) * ((double)rand() / (double)RAND_MAX);
            fprintf(file, "%lx\t", *(unsigned long *)(&(temps[j])));  // output format is hex
            // fprintf(file, "%le\n", (temps[j])); // output format is FP number, slower
        }
        fprintf(file, "\n");
    }
    // for(int idxFile = 0; idxFile < numOfFiles; idxFile++)
    // {
    // }

    // end everything
    free(intervals);
    free(temps);
    fclose(file);
    // for(int i = 0; i < dimension; i++)
    // {
    //     fclose(files[i]);
    // }
    // free(files);
    // free(filenameTmp);

    return 0;
}
