#include "common.h"
#include "mpi.h"

struct errorInfo {
    double sumError;
    double maxError;
    double maxInputX0;
};

#ifndef SUFFIX
#define SUFFIX orgin
#endif
#ifndef EXPRESSION
#define EXPRESSION sum
#endif

#define EXPRESSIONMINE ADDSUFFIX(EXPRESSION, SUFFIX)
#define SUFFIX1 mpfr
#define EXPRESSIONMPFR ADDSUFFIX(EXPRESSION, SUFFIX1)

#define TESTNUMX0 500000
// #define FP
// #define DEBUG
double EXPRESSIONMPFR(double, mpfr_t);
double EXPRESSIONMINE(double);

int computeOrcle1param(double x0, mpfr_t orcle)
{
    return EXPRESSIONMPFR(x0, orcle);
}

int computeResult1param(double x0, mpfr_t mpfrResult)
{
    int status = 1;

    double result = EXPRESSIONMINE(x0);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}

struct errorInfo test1paramParallel(DL x0Start, DL x0End, double stepX0, const char* fileName, int myid, double xx0)
{
    // printf("myid = %d: x0Start: %lg, x0End: %lg\n", myid, x0Start.d, x0End.d);
    DL x0, maxInputX0;
    #ifdef DEBUG
    DL orcle, result;
    #endif
    int i0, flag;
    double reUlp, sumError, aveReUlp, maxReUlp, lenX0;

    // mpfr
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // file
    // char *suffix = "sample.txt";
    // char *fileNameSample;
    // FILE *f;
    // fileNameSample = (char *) calloc(strlen(fileName) + strlen(suffix) + 128, sizeof(char));
    // sprintf(fileNameSample, "./outputs/%s_%s_%d", fileName, suffix, myid);
    // // printf("%s\n", fileNameSample);
    // if ((f = fopen(fileNameSample, "w")) == NULL) { 
    //     printf("Error opening file %s.\n", fileNameSample);
    //     exit(0);
    // }

    // Real number average
    maxReUlp = 0;
    flag = 1;
    size_t testCount = 0;
    sumError = 0;
    double x0RealStart = xx0 + ceil((x0Start.d - xx0) / stepX0) * stepX0;
    for(x0.d = x0RealStart; x0.d < x0End.d; x0.d += stepX0) {
        testCount++;
        computeResult1param(x0.d, mpfrResult);
        computeOrcle1param(x0.d, mpfrOrcle);
        #ifdef SINGLE
        reUlp = computeUlpDiffF(mpfrOrcle, mpfrResult);
        #else   // compute Double ULP as default
        reUlp = computeUlpDiff(mpfrOrcle, mpfrResult);
        #endif
        // if(reUlp <= 0.5) {
        //     reUlp = 0;
        // }
        // if(isfinite(reUlp) == 0) {
        //     printf("happen to NaN or inf\n");
        //     exit(1);
        // }
        // fprintf(f, "%.16le\n", reUlp);
        sumError += reUlp;
        if (reUlp > maxReUlp) {
            flag = 0;
            maxInputX0 = x0;
            maxReUlp = reUlp;
        }
    }
    // fprintf(f, "\n");
    // aveReUlp = sumError / testCount;
    // if(flag == 1) {
    //     printf("all error are 0!!\n");
    // }
    // printf("average ulp\tmax ulp\n");
    // printf("%lg\t%lg\n", aveReUlp, maxReUlp);
    // printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxReUlp);

    // fclose(f);
    struct errorInfo err;
    err.sumError = sumError;
    err.maxError = maxReUlp;
    err.maxInputX0 = maxInputX0.d;
    return err;
}

int main(int argc, char **argv) {
    // parallel
    int myid, numProcs;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Datatype MPI_errorInfo;
    MPI_Type_contiguous(3, MPI_DOUBLE, &MPI_errorInfo); 
    MPI_Type_commit(&MPI_errorInfo);

    // parameters init
    DL x0Start, x0End;
    unsigned long int testNumX0;
    x0Start.d = 1;
    x0End.d = 2;
    
    testNumX0 = TESTNUMX0;
    
    char *fileNameKernel;
    fileNameKernel = calloc(256, sizeof(char));
    if(argc == 5) {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        testNumX0 = strtod(argv[3], NULL);
        strcpy(fileNameKernel, argv[4]);
    } else if(argc == 4) {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        strcpy(fileNameKernel, argv[3]);
    } else if(argc == 3) {
        testNumX0 = strtod(argv[1], NULL);
        strcpy(fileNameKernel, argv[4]);
    } else {
        printf("Usage: ./test1paramParallel.exe [x0Start x0End testNumX0 fileNameKernel]\n");
        printf("Usage: if no correct input:\n");
        printf("Usage: \tthe fixed inputs [%g %g %lu] will be used\n", x0Start.d, x0End.d, testNumX0);
    }
    double lenX0 = x0End.d - x0Start.d;

    double stepX0 = lenX0 / testNumX0;

    if(myid == 0) {
        printf("\n---------------------------------------------------start test1paramParallel\n");
        printf("Parameters: x0Start: %lg, x0End: %lg, testNumX0 = %lu, fileNameKernel: %s\n", x0Start.d, x0End.d, testNumX0, fileNameKernel);
    }

    // local parameters init
    double lenX0Local = lenX0 / numProcs;
    DL x0StartLocal;
    x0StartLocal.d = x0Start.d + myid * lenX0Local;
    DL x0EndLocal;
    x0EndLocal.d = x0Start.d + (myid + 1) * lenX0Local;
    // if(myid != numProcs - 1) {
    //     x0EndLocal.d = x0Start.d + (myid + 1) * lenX0Local;
    // } else {
    //     x0EndLocal.d = x0End.d;
    // }

    // call the error test function
    struct errorInfo err = test1paramParallel(x0StartLocal, x0EndLocal, stepX0,  fileNameKernel, myid, x0Start.d);
    
    // gather errors and find the max
    struct errorInfo *errs;
    errs = (struct errorInfo *)calloc(numProcs, sizeof(struct errorInfo));
    MPI_Gather(&err, 1, MPI_errorInfo, errs, 1, MPI_errorInfo, 0, MPI_COMM_WORLD);
    if(myid == 0)
    {
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
            aveError += errs[i].sumError;
        }
        aveError = aveError / (testNumX0);
        DL maxInputX0;
        maxInputX0.d = errs[maxErrorIdx].maxInputX0;

        FILE *fErr;
        char *directory = "./outputs";
        char *suffixErr = "error.txt";
        char *fileNameErr;
        fileNameErr = (char *) calloc(strlen(directory) + strlen(fileNameKernel) + strlen(suffixErr) + 128, sizeof(char));
        sprintf(fileNameErr, "%s/%s_%s", directory, fileNameKernel, suffixErr);
        if ((fErr = fopen(fileNameErr, "w")) == NULL)
        {
            printf("Error opening file %s.\n", fileNameErr);
            exit(0);
        }
        printf("average ulp\tmax ulp\n");
        printf("%lg\t%lg\n", aveError, maxError);
        printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveError, maxInputX0.l, maxInputX0.d, maxError);
        fprintf(fErr, "average ulp\tmax ulp\n");
        fprintf(fErr, "%lg\t%lg\n", aveError, maxError);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveError, maxInputX0.l, maxInputX0.d, maxError);
    }
    
    // end everything
    MPI_Type_free(&MPI_errorInfo);
    MPI_Finalize();
    return 0;
}