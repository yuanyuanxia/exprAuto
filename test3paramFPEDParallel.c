#include "common.h"
#include "mpi.h"

struct errorInfo {
    double sumError;
    double maxError;
    double maxInputX0;
    double maxInputX1;
    double maxInputX2;
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

#define TESTNUMX0 256
#define TESTNUMX1 256
#define TESTNUMX2 256
// #define FP
// #define DEBUG
double EXPRESSIONMPFR(double, double, double, mpfr_t);
double EXPRESSIONMINE(double, double, double);

int computeOrcle3param(double x0, double x1, double x2, mpfr_t orcle) {
    return EXPRESSIONMPFR(x0, x1, x2, orcle);
}

int computeResult3param(double x0, double x1, double x2, mpfr_t mpfrResult) {
    int status = 1;

    double result = EXPRESSIONMINE(x0, x1, x2);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}

struct errorInfo test3paramFPEDParallel(DL x0Start, DL x0End, DL x1Start, DL x1End, DL x2Start, DL x2End, unsigned long int testNumX0, unsigned long int testNumX1, unsigned long int testNumX2, const char* fileNameKernel, int myid, int i2StartLocal, int i2EndLocal) {
    // printf("myid = %d: x0Start: %lg, x0End: %lg, x1Start: %lg, x1End: %lg, x2Start: %lg, x2End: %lg\n", myid, x0Start.d, x0End.d, x1Start.d, x1End.d, x2Start.d, x2End.d);
    DL maxInputX0, maxInputX1, maxInputX2;
    int i0, i1, i2;
    // int flag;
    double x0, x1, x2, reUlp, sumError, aveReUlp, maxReUlp, lenX0, lenX1, lenX2;
    
    // mpfr
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // file
    char *directory = "outputs";
    char *suffix = "sample";
    char *fileNameSample;
    FILE *f;
    fileNameSample = (char *) calloc(strlen(fileNameKernel) + strlen(suffix) + 128, sizeof(char));
    sprintf(fileNameSample, "./%s/%s_%s_%d.txt", outputs, fileNameKernel, suffix, myid);
    printf("%s\n", fileNameSample);
    if ((f = fopen(fileNameSample, "w")) == NULL) { 
        printf("Error opening file %s.\n", fileNameSample);
        exit(0);
    }

    // loop boundary
    lenX0 = x0End.d - x0Start.d;
    lenX1 = x1End.d - x1Start.d;
    lenX2 = x2End.d - x2Start.d;
    double stepX0 = lenX0 / (double)testNumX0;
    double stepX1 = lenX1 / (double)testNumX1;
    double stepX2 = lenX2 / (double)testNumX2;

    // Real number average
    maxReUlp = 0;
    // flag = 1;
    // size_t testCount = 0;
    sumError = 0;
    for(i2 = i2StartLocal; i2 <= i2EndLocal; i2++) {
        x2 = x2Start.d + stepX2 * i2;
        for(i1 = 0; i1 <= (int)testNumX1; i1++) {
            x1 = x1Start.d + stepX1 * i1;
            for(i0 = 0; i0 <= (int)testNumX0; i0++) {
                x0 = x0Start.d + stepX0 * i0;
                computeResult3param(x0, x1, x2, mpfrResult);
                computeOrcle3param(x0, x1, x2, mpfrOrcle);
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
                fprintf(f, "%le\t%e\n", x0, reUlp);
                sumError += reUlp;
                if (reUlp > maxReUlp) {
                    // flag = 0;
                    maxInputX2.d = x2;
                    maxInputX1.d = x1;
                    maxInputX0.d = x0;
                    maxReUlp = reUlp;
                }
            }
        }   
    }
    // fprintf(f, "\n");
    // aveReUlp = sumError / testCount;
    // if(flag == 1) {
    //     printf("all error are 0!!\n");
    // }
    // printf("average ulp\tmax ulp\n");
    // printf("%lg\t%lg\n", aveReUlp, maxReUlp);
    // printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxInputX2 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxInputX2.l, maxInputX2.d, maxReUlp);

    fclose(f);
    struct errorInfo err;
    err.sumError = sumError;
    err.maxError = maxReUlp;
    err.maxInputX0 = maxInputX0.d;
    err.maxInputX1 = maxInputX1.d;
    err.maxInputX2 = maxInputX2.d;
    return err;
}

int main(int argc, char **argv) {
    // parallel
    int myid, numProcs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Datatype MPI_errorInfo;
    MPI_Type_contiguous(5, MPI_DOUBLE, &MPI_errorInfo); 
    MPI_Type_commit(&MPI_errorInfo);

    // parameters init
    DL x0Start, x0End, x1Start, x1End, x2Start, x2End;
    unsigned long int testNumX0, testNumX1, testNumX2;
    x0Start.d = 1;
    x0End.d = 2;
    x1Start.d = 1;
    x1End.d = 2;
    x2Start.d = 1;
    x2End.d = 2;
    testNumX0 = TESTNUMX0;
    testNumX1 = TESTNUMX1;
    testNumX2 = TESTNUMX2;
    char *fileNameKernel;
    fileNameKernel = calloc(256, sizeof(char));
    if(argc == 11) {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        x1Start.d = strtod(argv[3], NULL);
        x1End.d = strtod(argv[4], NULL);
        x2Start.d = strtod(argv[5], NULL);
        x2End.d = strtod(argv[6], NULL);
        testNumX0 = strtod(argv[7], NULL);
        testNumX1 = strtod(argv[8], NULL);
        testNumX2 = strtod(argv[9], NULL);
        strcpy(fileNameKernel, argv[10]);
    } else if(argc == 8) {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        x1Start.d = strtod(argv[3], NULL);
        x1End.d = strtod(argv[4], NULL);
        x2Start.d = strtod(argv[5], NULL);
        x2End.d = strtod(argv[6], NULL);
        strcpy(fileNameKernel, argv[7]);
    } else if(argc == 5) {
        testNumX0 = strtod(argv[1], NULL);
        testNumX1 = strtod(argv[2], NULL);
        testNumX2 = strtod(argv[3], NULL);
        strcpy(fileNameKernel, argv[4]);
    } else {
        printf("Usage: ./test3paramFPEDParallel.exe [x0Start x0End x1Start x1End x2Start x2End testNumX0 testNumX1 testNumX2 fileNameKernel]\n");
        printf("Usage: if no correct input:\n");
        printf("Usage: \tthe fixed inputs [%g %g %g %g %g %g %lu %lu %lu] will be used\n", x0Start.d, x0End.d, x1Start.d, x1End.d, x2Start.d, x2End.d, testNumX0, testNumX1, testNumX2);
    }

    if(myid == 0) {
        printf("\n---------------------------------------------------start test3param\n");
        printf("Parameters: x0Start: %lg, x0End: %lg, x1Start: %lg, x1End: %lg, x2Start: %lg, x2End: %lg, testNumX0 = %lu, testNumX1 = %lu, testNumX2 = %lu, fileNameKernel: %s\n", x0Start.d, x0End.d, x1Start.d, x1End.d, x2Start.d, x2End.d, testNumX0, testNumX1, testNumX2, fileNameKernel);
    }

    // local parameters init
    int lenX2Local = testNumX2 / numProcs;
    int i2StartLocal;
    i2StartLocal = myid * lenX2Local;
    int i2EndLocal;
    if(myid != numProcs - 1) {
        i2EndLocal = (myid + 1) * lenX2Local - 1;
    } else {
        i2EndLocal = testNumX2;
    }

    // call the error test function
    struct errorInfo err = test3paramFPEDParallel(x0Start, x0End, x1Start, x1End, x2Start, x2End, testNumX0, testNumX1, testNumX2, fileNameKernel, myid, i2StartLocal, i2EndLocal);

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
        aveError = aveError / (testNumX0 * testNumX1 * testNumX2);
        DL maxInputX0, maxInputX1, maxInputX2;
        maxInputX0.d = errs[maxErrorIdx].maxInputX0;
        maxInputX1.d = errs[maxErrorIdx].maxInputX1;
        maxInputX2.d = errs[maxErrorIdx].maxInputX2;

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
        printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxInputX2 = 0x%016lx %lg, maxReUlp = %lg\n", aveError, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxInputX2.l, maxInputX2.d, maxError);
        fprintf(fErr, "average ulp\tmax ulp\n");
        fprintf(fErr, "%lg\t%lg\n", aveError, maxError);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxInputX2 = 0x%016lx %lg, maxReUlp = %lg\n", aveError, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxInputX2.l, maxInputX2.d, maxError);
    }
    
    // end everything
    MPI_Type_free(&MPI_errorInfo);
    MPI_Finalize();
    return 0;
}