#include "common.h"

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

void test3param(DL x0Start, DL x0End, DL x1Start, DL x1End, DL x2Start, DL x2End, unsigned long int testNumX0, unsigned long int testNumX1, unsigned long int testNumX2, const char* fileNameKernel) {
    DL ii0, ii1, ii2, maxInputX0, maxInputX1, maxInputX2;
    #ifdef DEBUG
    DL orcle, result;
    #endif
    int i0, i1, i2, flag;
    double x0, x1, x2, reUlp, aveReUlp, maxReUlp, lenX0, lenX1, lenX2;
    #ifdef FP
    unsigned long int reUlpInt, stepX0, stepX1, stepX2, sum, sumY, aveReUlpInt, maxReUlpInt;
    #endif
    char *directory = "./outputs";
    FILE *f;
    char *suffix = "sample.txt";
    char *fileNameSample;
    FILE *fErr;
    char *suffixErr = "error.txt";
    char *fileNameErr;

    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // printf("test expression: %s\n", STR2(EXPRESSION));
    // printf("x0Start  : %lg 0x%016lx\nx0End    : %lg 0x%016lx\nx1Start  : %lg 0x%016lx\nx1End    : %lg 0x%016lx\n", x0Start.d, x0Start.l, x0End.d, x0End.l, x1Start.d, x1Start.l, x1End.d, x1End.l);
    // fprintf(f, "x0Start  : %lg 0x%016lx\nx0End    : %lg 0x%016lx\nx1Start  : %lg 0x%016lx\nx1End    : %lg 0x%016lx\n", x0Start.d, x0Start.l, x0End.d, x0End.l, x1Start.d, x1Start.l, x1End.d, x1End.l);
    // fprintf(f, "\nxInput\t\txInput (Hex)\t\tyInput\t\tyInput (Hex)\t\tresult\t\tresult (Hex)\t\torcle\t\torcle (Hex)\t\tulp error\n");
    // printf("testNum : %lu 0x%lx\n", testNumX0 * testNumX1, testNumX0 * testNumX1);
    fileNameSample = (char *) calloc(strlen(fileNameKernel) + strlen(suffix) + 128, sizeof(char));
    sprintf(fileNameSample, "./outputs/%s_%s", fileNameKernel, suffix);
    printf("%s\n", fileNameSample);
    if ((f = fopen(fileNameSample, "w")) == NULL) { 
        printf("Error opening file %s.\n", fileNameSample);
        exit(0);
    }
    fileNameErr = (char *) calloc(strlen(fileNameKernel) + strlen(suffixErr) + 128, sizeof(char));
    sprintf(fileNameErr, "./outputs/%s_%s", fileNameKernel, suffixErr);
    printf("%s\n", fileNameErr);
    if ((fErr = fopen(fileNameErr, "w")) == NULL) { 
        printf("Error opening file %s.\n", fileNameErr);
        exit(0);
    }

    #ifdef FP   // FP number average
    // printf("FP number average\n");
    sum = x0End.l - x0Start.l;
    stepX0 = sum / testNumX0;
    if(stepX0 == 0) {
        stepX0 = 1;
    }
    sumY = x1End.l - x1Start.l;
    stepX1 = sum / testNumX1;
    if(stepX1 == 0) {
        stepX1 = 1;
    }

    printf("sum     : %lu 0x%lx\nstep    : %lu 0x%lx\ntestNum : %lu 0x%lx\n", sum, sum, stepX0, stepX0, testNumX0 * testNumX1, testNumX0 * testNumX1);
    // fprintf(f, "sum     : %lu 0x%lx\nstep    : %lu 0x%lx\ntestNum : %lu 0x%lx\n", sum, sum, stepX0, stepX0, testNumX0 * testNumX1, testNumX0 * testNumX1);
    for(ii2.l = x2Start.l; ii2.l <= x2End.l; ii2.l = ii2.l + stepX2) {
        x2 = ii2.d;
        for(ii1.l = x1Start.l; ii1.l <= x1End.l; ii1.l = ii1.l + stepX1) {
            x1 = ii1.d;
            for(ii0.l = zStart.l; ii0.l <= zEnd.l; ii0.l = ii0.l + stepX0) {
                x0 = ii0.d;

    #else   // Real number average
    // printf("Real number average\n");
    lenX0 = x0End.d - x0Start.d;
    lenX1 = x1End.d - x1Start.d;
    lenX2 = x2End.d - x2Start.d;
    maxReUlp = 0;
    flag = 1;
    for(i2 = 0; i2 <= testNumX2; i2++) {
        ii2.d = x2Start.d + lenX2 / (double)testNumX2 * i2;
        x2 = ii2.d;
        for(i1 = 0; i1 <= testNumX1; i1++) {
            ii1.d = x1Start.d + lenX1 / (double)testNumX1 * i1;
            x1 = ii1.d;
            for(i0 = 0; i0 <= testNumX0; i0++) {
                ii0.d = x0Start.d + lenX0 / (double)testNumX0 * i0;
                x0 = ii0.d;
    #endif
                computeResult3param(x0, x1, x2, mpfrResult);
                computeOrcle3param(x0, x1, x2, mpfrOrcle);
                #ifdef SINGLE
                reUlp = computeUlpDiffF(mpfrOrcle, mpfrResult);
                #else   // compute Double ULP as default
                reUlp = computeUlpDiff(mpfrOrcle, mpfrResult);
                #endif
                #ifdef DEBUG
                orcle.d = mpfr_get_d(mpfrOrcle, MPFR_RNDN);
                result.d = mpfr_get_d(mpfrResult, MPFR_RNDN);
                // fprintf(f, "%le\t%016lx\t%le\t%016lx\t%le\t%016lx\t%le\t%016lx\t%.16le\n", ii1.d, ii1.l, ii0.d, ii0.l, result.d, result.l, orcle.d, orcle.l, reUlp);
                // printf("%le\t%016lx\t%le\t%016lx\t%le\t%016lx\t%le\t%016lx\t%.16le\n", ii1.d, ii1.l, ii0.d, ii0.l, result.d, result.l, orcle.d, orcle.l, reUlp);
                printf("%8lg %8lg %8lg %8lg %8lg %8lg\n", ii2.d, ii1.d, ii0.d, result.d, orcle.d, reUlp);
                #endif
                // if(reUlp <= 0.5) {
                //     reUlp = 0;
                // }
                // if(isfinite(reUlp) == 0) {
                //     printf("happen to NaN or inf\n");
                //     exit(1);
                // }
                fprintf(f, "%.16le\n", reUlp);
                aveReUlp += reUlp;
                if (reUlp > maxReUlp) {
                    flag = 0;
                    maxInputX2 = ii2;
                    maxInputX1 = ii1;
                    maxInputX0 = ii0;
                    maxReUlp = reUlp;
                }
            }
        }   
    }
    // fprintf(f, "\n");
    aveReUlp = aveReUlp / (testNumX0 * testNumX1 * testNumX2);
    if(flag == 1) {
        printf("all error are 0!!\n");
    } else {
        printf("average ulp\tmax ulp\n");
        printf("%lg\t%lg\n", aveReUlp, maxReUlp);
        printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxInputX2 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxInputX2.l, maxInputX2.d, maxReUlp);
        fprintf(fErr, "average ulp\tmax ulp\n");
        fprintf(fErr, "%lg\t%lg\n", aveReUlp, maxReUlp);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxInputX2 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxInputX2.l, maxInputX2.d, maxReUlp);
    }

    fclose(f);
    fclose(fErr);
    free(fileNameSample);
    free(fileNameErr);
}

int main(int argc, char **argv) {
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
        printf("Usage: ./test3param.exe [x0Start x0End x1Start x1End x2Start x2End testNumX0 testNumX1 testNumX2 fileNameKernel]\n");
        printf("Usage: if no correct input:\n");
        printf("Usage: \tthe fixed inputs [%g %g %g %g %g %g %lu %lu %lu] will be used\n", x0Start.d, x0End.d, x1Start.d, x1End.d, x2Start.d, x2End.d, testNumX0, testNumX1, testNumX2);
    }
    printf("\n---------------------------------------------------start test3param\n");
    printf("Parameters: x0Start: %lg, x0End: %lg, x1Start: %lg, x1End: %lg, x2Start: %lg, x2End: %lg, testNumX0 = %lu, testNumX1 = %lu, testNumX2 = %lu, fileNameKernel: %s\n", x0Start.d, x0End.d, x1Start.d, x1End.d, x2Start.d, x2End.d, testNumX0, testNumX1, testNumX2, fileNameKernel);

    test3param(x0Start, x0End, x1Start, x1End, x2Start, x2End, testNumX0, testNumX1, testNumX2, fileNameKernel);

    return 0;
}