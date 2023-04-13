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
    double maxInputX1;
    double maxInputX2;
    double maxInputX3;
    double maxInputX4;
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

#define TESTNUMX0 24
#define TESTNUMX1 24
#define TESTNUMX2 24
#define TESTNUMX3 24
#define TESTNUMX4 24
// #define FP
// #define DEBUG
#ifndef ERRFILE
#define ERRFILE 0
#endif
double EXPRESSIONMPFR(double, double, double, double, double, mpfr_t);
double EXPRESSIONMINE(double, double, double, double, double);

int computeOrcle5param(double x0, double x1, double x2, double x3, double x4, mpfr_t orcle) {
    return EXPRESSIONMPFR(x0, x1, x2, x3, x4, orcle);
}

int computeResult5param(double x0, double x1, double x2, double x3, double x4, mpfr_t mpfrResult) {
    int status = 1;

    double result = EXPRESSIONMINE(x0, x1, x2, x3, x4);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}
}

struct errorInfo test5paramFPEDParallel(unsigned long int testNumX0, unsigned long int testNumX1, unsigned long int testNumX2, unsigned long int testNumX3, unsigned long int testNumX4, const char* uniqueLabel, const char* fileNameKernel, int myid, int i0StartLocal, int i0EndLocal, int i1StartLocal, int i1EndLocal, int i2StartLocal, int i2EndLocal, int i3StartLocal, int i3EndLocal, int i4StartLocal, int i4EndLocal, double x0startOriginInterval, double x1startOriginInterval, double x2startOriginInterval, double x3startOriginInterval, double x4startOriginInterval, double stepX0, double stepX1, double stepX2, double stepX3, double stepX4) {
    DL maxInputX0, maxInputX1, maxInputX2, maxInputX3, maxInputX4;
    int i0, i1, i2, i3, i4;
    // int flag;
    double x0, x1, x2, x3, x4, reUlp, sumError, aveReUlp, maxReUlp, lenX0, lenX1, lenX2, lenX3, lenX4;
    
    // mpfr
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // write error data to file
    #if ERRFILE
    char *directory = "outputs";
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
    #endif

    // Real number average
    maxReUlp = 0;
    // flag = 1;
    // size_t testCount = 0;
    sumError = 0;
    for(i4 = i4StartLocal; i4 <= i4EndLocal; i4++) {
        x4 = x4startOriginInterval + stepX4 * i4;
        for(i3 = i3StartLocal; i3 <= i3EndLocal; i3++) {
            x3 = x3startOriginInterval + stepX3 * i3;
            for(i2 = i2StartLocal; i2 <= i2EndLocal; i2++) {
                x2 = x2startOriginInterval + stepX2 * i2;
                for(i1 = i1StartLocal; i1 <= i1EndLocal; i1++) {
                    x1 = x1startOriginInterval + stepX1 * i1;
                    for(i0 = i0StartLocal; i0 <= i0EndLocal; i0++) {
                        x0 = x0startOriginInterval + stepX0 * i0;
                        computeResult5param(x0, x1, x2, x3, x4, mpfrResult);
                        computeOrcle5param(x0, x1, x2, x3, x4, mpfrOrcle);
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
                        // fprintf(f, "%le\t%le\t%le\t%e\n", x0, x1, x2, reUlp);
                        if (isnormal(reUlp) != 0)
                        {
                            #if ERRFILE
                            fprintf(f, "%le\t%le\n", x0, reUlp);
                            #endif
                            sumError += reUlp;
                            if (reUlp > maxReUlp) {
                                // flag = 0;
                                maxInputX4.d = x4;
                                maxInputX3.d = x3;
                                maxInputX2.d = x2;
                                maxInputX1.d = x1;
                                maxInputX0.d = x0;
                                maxReUlp = reUlp;
                            }
                        }
                    }
                }   
            }
        }
    }
    // aveReUlp = sumError / testCount;
    // if(flag == 1) {
    //     printf("all error are 0!!\n");
    // }
    // printf("average ulp\tmax ulp\n");
    // printf("%lg\t%lg\n", aveReUlp, maxReUlp);
    // printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxInputX2 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxInputX2.l, maxInputX2.d, maxReUlp);
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
    err.maxInputX1 = maxInputX1.d;
    err.maxInputX2 = maxInputX2.d;
    err.maxInputX3 = maxInputX3.d;
    err.maxInputX4 = maxInputX4.d;
    return err;
}

int main_inner(const unsigned long int &testNumX0, const unsigned long int &testNumX1, const unsigned long int &testNumX2, const unsigned long int &testNumX3, const unsigned long int &testNumX4, const string &uniqueLabelStr, const string &fileNameKernelStr, const int &myid, const int &numProcs, const int &i0StartLocal, const int &i0EndLocal, const int &i1StartLocal, const int &i1EndLocal, const int &i2StartLocal, const int &i2EndLocal, const int &i3StartLocal, const int &i3EndLocal, const int &i4StartLocal, const int &i4EndLocal, const double &x0startOriginInterval, const double &x1startOriginInterval, const double &x2startOriginInterval, const double &x3startOriginInterval, const double &x4startOriginInterval, const double &stepX0, const double &stepX1, const double &stepX2, const double &stepX3, const double &stepX4)
{
    std::fesetround(FE_TONEAREST);
	std::cout << std::scientific << std::setprecision(6);
	iRRAM::cout << iRRAM::setRwidth(30);

    MPI_Datatype MPI_errorInfo;
    MPI_Type_contiguous(5, MPI_DOUBLE, &MPI_errorInfo); 
    MPI_Type_commit(&MPI_errorInfo);

    char *fileNameKernel;
    fileNameKernel = (char *)calloc(256, sizeof(char));
    sprintf(fileNameKernel, "%s", fileNameKernelStr.c_str());
    char *uniqueLabel;
    uniqueLabel = (char *)calloc(256, sizeof(char));
    sprintf(uniqueLabel, "%s", uniqueLabelStr.c_str());

    // call the error test function
    struct errorInfo err = test5paramFPEDParallel(testNumX0, testNumX1, testNumX2, testNumX3, testNumX4, uniqueLabel, fileNameKernel, myid, i0StartLocal, i0EndLocal, i1StartLocal, i1EndLocal, i2StartLocal, i2EndLocal, i3StartLocal, i3EndLocal, i4StartLocal, i4EndLocal, x0startOriginInterval, x1startOriginInterval, x2startOriginInterval, x3startOriginInterval, x4startOriginInterval, stepX0, stepX1, stepX2, stepX3, stepX4);

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
        aveError = aveError / (testNumX0 * testNumX1 * testNumX2 * testNumX3 * testNumX4);
        DL maxInputX0, maxInputX1, maxInputX2, maxInputX3, maxInputX4;
        maxInputX0.d = errs[maxErrorIdx].maxInputX0;
        maxInputX1.d = errs[maxErrorIdx].maxInputX1;
        maxInputX2.d = errs[maxErrorIdx].maxInputX2;
        maxInputX3.d = errs[maxErrorIdx].maxInputX3;
        maxInputX4.d = errs[maxErrorIdx].maxInputX4;

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
        // printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxInputX2 = 0x%016lx %lg, maxReUlp = %lg\n", aveError, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxInputX2.l, maxInputX2.d, maxError);
        fprintf(fErr, "average ulp\tmax ulp\n");
        fprintf(fErr, "%.16le\t%.16le\n", aveError, maxError);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxInputX2 = 0x%016lx %lg, maxInputX3 = 0x%016lx %lg, maxInputX4 = 0x%016lx %lg, maxReUlp = %lg\n", aveError, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxInputX2.l, maxInputX2.d, maxInputX3.l, maxInputX3.d, maxInputX4.l, maxInputX4.d, maxError);

        free(fileNameErr);
        free(uniqueLabel);
        fclose(fErr);
    }
    
    // clear
    free(fileNameKernel);
    free(errs);
    MPI_Type_free(&MPI_errorInfo);
}


int main(int argc, char **argv) {
    // parallel
    int myid, numProcs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // parameters init
    DL x0Start, x0End, x1Start, x1End, x2Start, x2End, x3Start, x3End, x4Start, x4End;
    unsigned long int testNumX0, testNumX1, testNumX2, testNumX3, testNumX4;
    x0Start.d = 1;
    x0End.d = 2;
    x1Start.d = 1;
    x1End.d = 2;
    x2Start.d = 1;
    x2End.d = 2;
    x3Start.d = 1;
    x3End.d = 2;
    x4Start.d = 1;
    x4End.d = 2;
    int x0startNowIdx, x1startNowIdx, x2startNowIdx, x3startNowIdx, x4startNowIdx;
    double x0startOriginInterval, x1startOriginInterval, x2startOriginInterval, x3startOriginInterval, x4startOriginInterval;
    double stepX0, stepX1, stepX2, stepX3, stepX4;

    testNumX0 = TESTNUMX0;
    testNumX1 = TESTNUMX1;
    testNumX2 = TESTNUMX2;
    testNumX3 = TESTNUMX3;
    testNumX4 = TESTNUMX4;

    char *fileNameKernel;
    fileNameKernel = (char *)calloc(256, sizeof(char));
    char *uniqueLabel;
    uniqueLabel = (char *)calloc(256, sizeof(char));
    if(argc == 33) {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        x1Start.d = strtod(argv[3], NULL);
        x1End.d = strtod(argv[4], NULL);
        x2Start.d = strtod(argv[5], NULL);
        x2End.d = strtod(argv[6], NULL);
        x3Start.d = strtod(argv[7], NULL);
        x3End.d = strtod(argv[8], NULL);
        x4Start.d = strtod(argv[9], NULL);
        x4End.d = strtod(argv[10], NULL);
        testNumX0 = strtod(argv[11], NULL);
        testNumX1 = strtod(argv[12], NULL);
        testNumX2 = strtod(argv[13], NULL);
        testNumX3 = strtod(argv[14], NULL);
        testNumX4 = strtod(argv[15], NULL);
        x0startNowIdx = atoi(argv[16]);
        x1startNowIdx = atoi(argv[17]);
        x2startNowIdx = atoi(argv[18]);
        x3startNowIdx = atoi(argv[19]);
        x4startNowIdx = atoi(argv[20]);
        x0startOriginInterval = strtod(argv[21], NULL);
        x1startOriginInterval = strtod(argv[22], NULL);
        x2startOriginInterval = strtod(argv[23], NULL);
        x3startOriginInterval = strtod(argv[24], NULL);
        x4startOriginInterval = strtod(argv[25], NULL);
        stepX0 = strtod(argv[26], NULL);
        stepX1 = strtod(argv[27], NULL);
        stepX2 = strtod(argv[28], NULL);
        stepX3 = strtod(argv[29], NULL);
        stepX4 = strtod(argv[30], NULL);
        strcpy(fileNameKernel, argv[31]);
        strcpy(uniqueLabel, argv[32]);
    // } else if(argc == 18) {
    //     x0Start.d = strtod(argv[1], NULL);
    //     x0End.d = strtod(argv[2], NULL);
    //     x1Start.d = strtod(argv[3], NULL);
    //     x1End.d = strtod(argv[4], NULL);
    //     x2Start.d = strtod(argv[5], NULL);
    //     x2End.d = strtod(argv[6], NULL);
    //     x0startNowIdx = atoi(argv[7]);
    //     x1startNowIdx = atoi(argv[8]);
    //     x2startNowIdx = atoi(argv[9]);
    //     x0startOriginInterval = strtod(argv[10], NULL);
    //     x1startOriginInterval = strtod(argv[11], NULL);
    //     x2startOriginInterval = strtod(argv[12], NULL);
    //     stepX0 = strtod(argv[13], NULL);
    //     stepX1 = strtod(argv[14], NULL);
    //     stepX2 = strtod(argv[15], NULL);
    //     strcpy(fileNameKernel, argv[16]);
    //     strcpy(uniqueLabel, argv[17]);
    // } else if(argc == 15) {
    //     testNumX0 = strtod(argv[1], NULL);
    //     testNumX1 = strtod(argv[2], NULL);
    //     testNumX2 = strtod(argv[3], NULL);
    //     x0startNowIdx = atoi(argv[4]);
    //     x1startNowIdx = atoi(argv[5]);
    //     x2startNowIdx = atoi(argv[6]);
    //     x0startOriginInterval = strtod(argv[7], NULL);
    //     x1startOriginInterval = strtod(argv[8], NULL);
    //     x2startOriginInterval = strtod(argv[9], NULL);
    //     stepX0 = strtod(argv[10], NULL);
    //     stepX1 = strtod(argv[11], NULL);
    //     stepX2 = strtod(argv[12], NULL);
    //     strcpy(fileNameKernel, argv[13]);
    //     strcpy(uniqueLabel, argv[14]);
    } else {
        printf("Usage: ./test3paramFPEDParallel.exe [x0Start x0End x1Start x1End x2Start x2End testNumX0 testNumX1 testNumX2 fileNameKernel]\n");
        printf("Usage: if no correct input:\n");
        printf("Usage: \tthe fixed inputs [%g %g %g %g %g %g %g %g %lu %lu %lu %lu] will be used\n", x0Start.d, x0End.d, x1Start.d, x1End.d, x2Start.d, x2End.d, x3Start.d, x3End.d, testNumX0, testNumX1, testNumX2, testNumX3);
    }

    // if(myid == 0) {
    //     printf("\n---------------------------------------------------start test3paramFPEDParallel\n");
    //     printf("Parameters: x0Start: %lg, x0End: %lg, x1Start: %lg, x1End: %lg, x2Start: %lg, x2End: %lg, testNumX0 = %lu, testNumX1 = %lu, testNumX2 = %lu, fileNameKernel: %s\n", x0Start.d, x0End.d, x1Start.d, x1End.d, x2Start.d, x2End.d, testNumX0, testNumX1, testNumX2, fileNameKernel);
    // }

    // local parameters init
    int lenX4Local = testNumX4 / numProcs;
    int i4StartLocal;
    i4StartLocal = x4startNowIdx + myid * lenX4Local;
    int i4EndLocal;
    if(myid != numProcs - 1) {
        i4EndLocal = x4startNowIdx + (myid + 1) * lenX4Local - 1;
    } else {
        i4EndLocal = x4startNowIdx + testNumX4;
    }
    int i3StartLocal = x3startNowIdx;
    int i3EndLocal = x3startNowIdx + testNumX3;
    int i2StartLocal = x2startNowIdx;
    int i2EndLocal = x2startNowIdx + testNumX2;
    int i1StartLocal = x1startNowIdx;
    int i1EndLocal = x1startNowIdx + testNumX1;
    int i0StartLocal = x0startNowIdx;
    int i0EndLocal = x0startNowIdx + testNumX0;

    string uniqueLabelStr(uniqueLabel);
    string fileNameKernelStr(fileNameKernel);
	iRRAM_initialize(argc, argv);
	iRRAM::iRRAM_exec(main_inner, testNumX0, testNumX1, testNumX2, testNumX3, testNumX4, uniqueLabelStr, fileNameKernelStr, myid, numProcs, i0StartLocal, i0EndLocal, i1StartLocal, i1EndLocal, i2StartLocal, i2EndLocal, i3StartLocal, i3EndLocal, i4StartLocal, i4EndLocal, x0startOriginInterval, x1startOriginInterval, x2startOriginInterval, x3startOriginInterval, x4startOriginInterval, stepX0, stepX1, stepX2, stepX3, stepX4);

    MPI_Finalize();
    return 0;
}