#include "common.h"
#include "mpi.h"
#include "multi.h"

struct errorInfo
{
    double sumError;
    double maxError;
    MAXINPUT_PARA_DEF // declare variables maxInputx0, maxInputx1, ...
};

#ifndef SUFFIX
#define SUFFIX Origin
#endif
#ifndef EXPRESSION
#define EXPRESSION example // sin(x) + cos(y)
#endif

#define EXPRESSIONMINE ADDSUFFIX(EXPRESSION, SUFFIX)
#define SUFFIX1 Mpfr
#define EXPRESSIONMPFR ADDSUFFIX(EXPRESSION, SUFFIX1)

// #define FP
// #define DEBUG
double EXPRESSIONMPFR(PARA, mpfr_t);
double EXPRESSIONMINE(PARA);

int computeOrcle(PARA, mpfr_t orcle) { return EXPRESSIONMPFR(INPUT, orcle); }

int computeResult(PARA, mpfr_t mpfrResult)
{
    int status = 1;

    double result = EXPRESSIONMINE(INPUT);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}

struct errorInfo testMultiParallel(int myid, int testNumLocal)
{
    // printf("myid = %d\n", myid);
    // init
    double *inputData = (double *)calloc(DIMENSION * testNumLocal, sizeof(double));
    PARA_DEF // declare variables x0, x1, ...
    MAXINPUT_PARA_DEF // declare variables maxInputx0, maxInputx1, ...
    // mpfr
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr)0);

    // file
    char *inputFileName = (char *) calloc(256, sizeof(char));
    sprintf(inputFileName, "inputData_%02d.txt", myid);
    FILE *inputFile;
    if((inputFile = fopen(inputFileName, "r")) == NULL)
    {
        printf("Error opening file %s.\n", inputFileName);
        exit(EXIT_FAILURE);
    }

    // char *directory = "outputs";
    // char *suffix = "sample";
    // char *fileNameSample;
    // FILE *f;
    // fileNameSample = (char *) calloc(strlen(fileNameKernel) + strlen(suffix) + 128, sizeof(char));
    // sprintf(fileNameSample, "./%s/%s_%s_%d.txt", directory, fileNameKernel, suffix, myid);
    // printf("%s\n", fileNameSample);
    // if ((f = fopen(fileNameSample, "w")) == NULL) {
    //     printf("Error opening file %s.\n", fileNameSample);
    //     exit(0);
    // }

    double maxReUlp = 0;
    double sumError = 0;
    int idxData = 0;
    unsigned long inputTmp;
    for (int i = 0; i < testNumLocal; i++)
    {
        for(int j = 0; j < DIMENSION; j++)
        {
            fscanf(inputFile, "%lx\t", &inputTmp);
            inputData[idxData] = *((double *)(&inputTmp));
            idxData++;
        }
    }
    idxData = 0;
    double reUlp;
    for(int i = 0; i < testNumLocal; i++)
    {
        INPUTDATA_TO_INPUT // set input data to x0, x1, ...

        computeResult(INPUT, mpfrResult);
        computeOrcle(INPUT, mpfrOrcle);
        #ifdef SINGLE
        reUlp = computeUlpDiffF(mpfrOrcle, mpfrResult);
        #else  // compute Double ULP as default
        reUlp = computeUlpDiff(mpfrOrcle, mpfrResult);
        #endif
        // reUlp = 2.1 * i;
        // if(reUlp <= 0.5) {
        //     reUlp = 0;
        // }
        // if(isfinite(reUlp) == 0) {
        //     printf("happen to NaN or inf\n");
        //     exit(1);
        // }
        // fprintf(f, "%le\t%le\t%le\t%e\n", x0, x1, x2, reUlp); // TODO
        sumError += reUlp;
        if(reUlp > maxReUlp)
        {
            // flag = 0;
            maxReUlp = reUlp;
            INPUT_TO_MAXINPUT // set x0, x1,... to maxInputx0, maxInputx1, ...
        }
    }

    // fprintf(f, "\n");
    // if(flag == 1) {
    //     printf("all error are 0!!\n");
    // }

    // end everything
    // fclose(f);
    // free(fileNameSample);
    fclose(inputFile);
    mpfr_clears(mpfrOrcle, mpfrResult, (mpfr_ptr)0);
    // free(input);
    free(inputData);
    struct errorInfo err;
    err.sumError = sumError;
    // printf("myid = %d, sumError = %lf\n", myid, sumError);
    err.maxError = maxReUlp;
    MAXINPUT_TO_ERR // set maxInputx0, maxInputx1,... to err.maxInputx0, err.maxInputx1, ...
    return err; // remember free maxInput OR set errInfo as the parameter of this function
}

int main(int argc, char **argv)
{
    // parallel
    int myid, numProcs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Datatype MPI_errorInfo;
    MPI_Type_contiguous(2 + PARA_NUM, MPI_DOUBLE, &MPI_errorInfo);
    MPI_Type_commit(&MPI_errorInfo);

    // parameters init
    if(argc != 3)
    {
        printf("please enter the name of outputfile, the number of input.\n");
        exit(EXIT_FAILURE);
    }
    char *fileNameKernel;
    fileNameKernel = calloc(256, sizeof(char));
    strcpy(fileNameKernel, argv[1]);
    int testNum = atoi(argv[2]);
    int dimension = PARA_NUM;
    int testNumLocal = testNum / numProcs;

    // useless code
    // basic initail
    // int i, j;
    // int dimension = (argc - 1) / 2;
    // DL *intervals;
    // intervals = (DL *)calloc(dimension * 2, sizeof(double));
    // for(i = 0; i < dimension; i++)
    // {
    //     intervals[2 * i + 0].d = atof(argv[1 + 2 * i + 0]);
    //     intervals[2 * i + 1].d = atof(argv[1 + 2 * i + 1]);
    // }
    // unsigned long *testNum;
    // testNum = (unsigned long *)calloc(dimension, sizeof(unsigned long));
    // for(i = 0; i < dimension; i++)
    // {
    //     testNum[i].d = atof(argv[1 + 2 * dimension + i]);
    // }
    // if(myid == 0) {
    //     printf("\n---------------------------------------------------start testMultiParallel\n");
    //     for(i = 0; i < dimension; i++)
    //     {
    //         printf("dimension No.%d: [%d, %d], testNum = %d\n", i, intervals[2 *i + 0], intervals[2 * i + 1], testNum[i]);
    //     }
    // }

    // call the error test function
    struct errorInfo err = testMultiParallel(myid, testNumLocal);

    // gather errors and find the max
    struct errorInfo *errs;
    errs = (struct errorInfo *)calloc(numProcs, sizeof(struct errorInfo));
    MPI_Gather(&err, 1, MPI_errorInfo, errs, 1, MPI_errorInfo, 0, MPI_COMM_WORLD);
    if(myid == 0)
    {
        // char *hjw = MAXINPUT_PARA_DECLARE;
        // printf("%s\n", WJH);
        double maxError = -1;
        double aveError = 0;
        double errTmp = -1;
        int maxErrorIdx = -1;
        for(int i = 0; i < numProcs; i++)
        {
            errTmp = errs[i].maxError;
            if(errTmp > maxError)
            {
                maxError = errTmp;
                maxErrorIdx = i;
            }
            // printf("errs[%d].sumError = %lf\n", i, errs[i].sumError);
            aveError += errs[i].sumError;
        }
        aveError = aveError / testNum;
        MAXINPUT_PARA_DEF
        ERR_TO_MAXINPUT

        FILE *fErr;
        char *directory = "./outputs";
        char *suffixErr = "error.txt";
        char *fileNameErr;
        fileNameErr = (char *)calloc(strlen(directory) + strlen(fileNameKernel) + strlen(suffixErr) + 128, sizeof(char));
        sprintf(fileNameErr, "%s/%s_%s", directory, fileNameKernel, suffixErr);
        if((fErr = fopen(fileNameErr, "w")) == NULL)
        {
            printf("Error opening file %s.\n", fileNameErr);
            exit(0);
        }
        printf("average ulp\tmax ulp\n");
        printf("%lg\t%lg\n", aveError, maxError);
        MAXINPUT_PRINT
        fprintf(fErr, "average ulp\tmax ulp\n");
        fprintf(fErr, "%lg\t%lg\n", aveError, maxError);

        free(fileNameErr);
        fclose(fErr);
    }

    // clear
    free(fileNameKernel);
    free(errs);
    MPI_Type_free(&MPI_errorInfo);
    MPI_Finalize();
    return 0;
}