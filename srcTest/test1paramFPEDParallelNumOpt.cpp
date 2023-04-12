#include <iostream>
#include <iomanip>
#include <limits>
#include <cfenv>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpfr.h>
#include "iRRAM/lib.h"

using namespace std;
using namespace iRRAM;
// int iRRAM::MAXiterationnum = 30;
// bool iRRAM::enableReiterate = true;
// bool iRRAM::alwaysenableReiterate = true;

extern "C"
{
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
#define EXPRESSION NMSEproblem334
#endif

#define EXPRESSIONMINE ADDSUFFIX(EXPRESSION, SUFFIX)
#define SUFFIX1 mpfr
#define EXPRESSIONMPFR ADDSUFFIX(EXPRESSION, SUFFIX1)

#define TESTNUMX0 500000
// #define FP
// #define DEBUG
#ifndef ERRFILE
#define ERRFILE 0
#endif

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
}

struct errorInfo test1FPEDparamParallel(unsigned long int testNumX0, const char* uniqueLabel, const char* fileNameKernel, int myid, int i0StartLocal, int i0EndLocal, double x0startOriginInterval, double stepX0) {
    DL maxInputX0;
    int i0;
    // int flag;
    double x0, reUlp, sumError, aveReUlp, maxReUlp, lenX0;

    // mpfr
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // write error data to file
    #if ERRFILE
    char *directory = "./outputs";
    char *suffix = "sample";
    char *fileNameSample;
    FILE *f;
    fileNameSample = (char *) calloc(strlen(directory) + strlen(uniqueLabel) + strlen(fileNameKernel) + strlen(suffix) + 128, sizeof(char));
    sprintf(fileNameSample, "%s/%s/%s_%s_%02d.txt", directory, uniqueLabel, fileNameKernel, suffix, myid);
    // printf("%s\n", fileNameSample);
    if ((f = fopen(fileNameSample, "w")) == NULL) { 
        printf("Error opening file %s.\n", fileNameSample);
        exit(0);
    }

    // printf("test expression: %s\n", STR2(EXPRESSION));
    // printf("x0Start  : %lg 0x%016lx\nx0End    : %lg 0x%016lx\n", x0Start.d, x0Start.l, x0End.d, x0End.l);
    // fprintf(f, "x0Start  : %lg 0x%016lx\nx0End    : %lg 0x%016lx\n", x0Start.d, x0Start.l, x0End.d, x0End.l);
    // fprintf(f, "\nxInput\t\txInput (Hex)\t\tresult\t\tresult (Hex)\t\torcle\t\torcle (Hex)\t\tulp error\n");
    // printf("testNum : %lu 0x%lx\n", testNumX0, testNumX0);
    #endif

    // Real number average
    maxReUlp = 0;
    aveReUlp = 0;
    // flag = 1;
    sumError = 0;
    // for(i0 = 0; i0 < testNumX0; i0++) {
        // ii0.d = x0startOriginInterval.d + stepX0 * i0;
        // x0 = ii0.d;
    for(i0 = i0StartLocal; i0 <= i0EndLocal; i0++) {
        x0 = x0startOriginInterval + stepX0 * i0;
        computeResult1param(x0, mpfrResult);
        computeOrcle1param(x0, mpfrOrcle);
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
        
        if (isnormal(reUlp) != 0)
        {
            #if ERRFILE
            fprintf(f, "%le\t%le\n", x0, reUlp);
            #endif
            sumError += reUlp;
            if (reUlp > maxReUlp) {
                // flag = 0;
                maxInputX0.d = x0;
                maxReUlp = reUlp;
            }
        }
        
    }
    // aveReUlp = aveReUlp / (testNumX0);
    // if(flag == 1) {
    //     printf("all error are 0!!\n");
    // } else {
    //     printf("average ulp\tmax ulp\n");
    //     printf("%le\t%le\n", aveReUlp, maxReUlp);
    //     printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxReUlp);
    // }
    #if ERRFILE
    // fprintf(f, "\n");

    // clear
    fclose(f);
    free(fileNameSample);
    #endif
    mpfr_clears(mpfrOrcle, mpfrResult, (mpfr_ptr) 0);
    struct errorInfo err;
    err.sumError = sumError;
    err.maxError = maxReUlp;
    err.maxInputX0 = maxInputX0.d;
    return err;
}

int main_inner(const unsigned long int &testNumX0, const string &uniqueLabelStr, const string &fileNameKernelStr, const int &myid, const int &numProcs, const int &i0StartLocal, const int &i0EndLocal, const double &x0startOriginInterval, const double &stepX0)
{
    std::fesetround(FE_TONEAREST);
	std::cout << std::scientific << std::setprecision(6);
	iRRAM::cout << iRRAM::setRwidth(30);

    MPI_Datatype MPI_errorInfo;
    MPI_Type_contiguous(3, MPI_DOUBLE, &MPI_errorInfo); 
    MPI_Type_commit(&MPI_errorInfo);

    char *fileNameKernel;
    fileNameKernel = (char *)calloc(256, sizeof(char));
    sprintf(fileNameKernel, "%s", fileNameKernelStr.c_str());
    char *uniqueLabel;
    uniqueLabel = (char *)calloc(256, sizeof(char));
    sprintf(uniqueLabel, "%s", uniqueLabelStr.c_str());

    // call the error test function
    struct errorInfo err = test1FPEDparamParallel(testNumX0, uniqueLabel, fileNameKernel, myid, i0StartLocal, i0EndLocal, x0startOriginInterval, stepX0);
    
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

        std::string directory{"./outputs"};
        std::string suffixErr("error.txt");
        FILE *fErr;
        char *fileNameErr;
        fileNameErr = (char *) calloc(directory.length() + strlen(uniqueLabel) + strlen(fileNameKernel) + suffixErr.length() + 128, sizeof(char));
        sprintf(fileNameErr, "%s/%s/%s_%s", directory.c_str(), uniqueLabel, fileNameKernel, suffixErr.c_str());
        if ((fErr = fopen(fileNameErr, "w")) == NULL)
        {
            printf("Error opening file %s.\n", fileNameErr);
            exit(0);
        }
        printf("average ulp\tmax ulp\n");
        printf("%.16le\t%.16le\n", aveError, maxError);
        // printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveError, maxInputX0.l, maxInputX0.d, maxError);
        fprintf(fErr, "average ulp\tmax ulp\n");
        fprintf(fErr, "%.16le\t%.16le\n", aveError, maxError);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveError, maxInputX0.l, maxInputX0.d, maxError);

        free(fileNameErr);
        free(uniqueLabel);
        fclose(fErr);
    }
    
    // clear
    free(fileNameKernel);
    free(errs);
    MPI_Type_free(&MPI_errorInfo);

    return 0;
}

int main(int argc, char **argv)
{
    // parallel
    int myid, numProcs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // parameters init
    DL x0Start, x0End;
    unsigned long int testNumX0;
    x0Start.d = 1;
    x0End.d = 2;
    int x0startNowIdx;
    double x0startOriginInterval;
    double stepX0;

    testNumX0 = TESTNUMX0;

    char *fileNameKernel;
    fileNameKernel = (char *)calloc(256, sizeof(char));
    char *uniqueLabel;
    uniqueLabel = (char *)calloc(256, sizeof(char));
    if (argc == 9)
    {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        testNumX0 = strtod(argv[3], NULL);
        x0startNowIdx = atoi(argv[4]);
        x0startOriginInterval = strtod(argv[5], NULL);
        stepX0 = strtod(argv[6], NULL);
        strcpy(fileNameKernel, argv[7]);
        strcpy(uniqueLabel, argv[8]);
    }
    else if (argc == 8)
    {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        x0startNowIdx = atoi(argv[3]);
        x0startOriginInterval = strtod(argv[4], NULL);
        stepX0 = strtod(argv[5], NULL);
        strcpy(fileNameKernel, argv[6]);
        strcpy(uniqueLabel, argv[7]);
    }
    else if (argc == 7)
    {
        testNumX0 = strtod(argv[1], NULL);
        x0startNowIdx = atoi(argv[2]);
        x0startOriginInterval = strtod(argv[3], NULL);
        stepX0 = strtod(argv[4], NULL);
        strcpy(fileNameKernel, argv[5]);
        strcpy(uniqueLabel, argv[6]);
    }
    else
    {
        printf("Usage: ./test1paramFPEDParallel.exe [x0Start x0End testNumX0 fileNameKernel]\n");
        printf("Usage: if no correct input:\n");
        printf("Usage: \tthe fixed inputs [%g %g %lu] will be used\n", x0Start.d, x0End.d, testNumX0);
    }

    if(myid == 0) {
        printf("\n---------------------------------------------------start test1paramFPEDParallel\n");
        printf("Parameters: x0Start: %lg, x0End: %lg, testNumX0 = %lu, fileNameKernel: %s\n", x0Start.d, x0End.d, testNumX0, fileNameKernel);
    }

    // local parameters init
    int lenX0Local = testNumX0 / numProcs;
    int i0StartLocal;
    i0StartLocal = x0startNowIdx + myid * lenX0Local;
    int i0EndLocal;
    if(myid != numProcs - 1) {
        i0EndLocal = x0startNowIdx + (myid + 1) * lenX0Local - 1;
    } else {
        i0EndLocal = x0startNowIdx + testNumX0;
    }
    string uniqueLabelStr(uniqueLabel);
    string fileNameKernelStr(fileNameKernel);
	iRRAM_initialize(argc, argv);
	iRRAM::iRRAM_exec(main_inner, testNumX0, uniqueLabelStr, fileNameKernelStr, myid, numProcs, i0StartLocal, i0EndLocal, x0startOriginInterval, stepX0);

    MPI_Finalize();
	return 0;
}