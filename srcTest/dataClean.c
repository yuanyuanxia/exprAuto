#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 255
#define PERCENT 0.0005
#ifndef RUNTIME
#define RUNTIME 10000
#endif

int main(int argc, char *argv[])
{
    int row = 0, i = 0;
    long cycle = 0;
    FILE *data;
    long time_base[RUNTIME];
    long time_base_sum = 0;
    int start, end;

    if(argc == 2)
    {
        data = fopen(argv[1], "r");
    }
    else
    {
        printf("ERROR: Invalid arguments. argc should be 2, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    if(!data)
    {
        printf("Error in opening file.\r\n");
        exit(1);
    }

    // printf("----Starting data clean: %d----\n", RUNTIME);
    row = 0;
    while(fscanf(data, "%ld\n", &time_base[row]) != EOF)
    {
        row++;
    }
    start = row * PERCENT;
    end = row - start;
    // printf("the data size is %d, we choose: %d to %d\n", row, start, end);
    for(i = start; i < end; i++)
    {
        time_base_sum += time_base[i];
    }
    // printf("----End data clean----\n");

    cycle = time_base_sum / (end - start);
    // printf("the performance result (cycle): %ld\n", cycle);
    printf("%ld\n", cycle);

    fclose(data);
}
