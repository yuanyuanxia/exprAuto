#include "common.h"

#ifndef SUFFIX
#define SUFFIX orgin
#endif
#ifndef EXPRESSION
#define EXPRESSION floudas2
#endif

#define EXPRESSIONMINE ADDSUFFIX(EXPRESSION, SUFFIX)
#define SUFFIX1 mpfr
#define EXPRESSIONMPFR ADDSUFFIX(EXPRESSION, SUFFIX1)

#define TESTNUMX0 1024
#define TESTNUMX1 1024
// #define FP
double EXPRESSIONMPFR(double, double, mpfr_t);
double EXPRESSIONMINE(double, double);

int computeOrcle2param(double x0, double x1, mpfr_t orcle) { return EXPRESSIONMPFR(x0, x1, orcle); }

int computeResult2param(double x0, double x1, mpfr_t mpfrResult) {
    int status = 1;

    double result = EXPRESSIONMINE(x0, x1);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}

void test2param(DL xStart, DL xEnd, DL yStart, DL yEnd, unsigned long int testNumX, unsigned long int testNumY, const char *fileNameKernel) {
    DL i, j, maxInputX, maxInputY;
    // DL orcle, result;
    int ii, jj, flag;
    double x, y, reUlp, aveReUlp, maxReUlp, lenX, lenY;
    #ifdef FP
    unsigned long int reUlpInt, step, stepY, sum, sumY, aveReUlpInt, maxReUlpInt;
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

    fileNameSample = (char *)calloc(strlen(fileNameKernel) + strlen(suffix) + 128, sizeof(char));
    sprintf(fileNameSample, "./outputs/%s_%s", fileNameKernel, suffix);
    printf("%s\n", fileNameSample);
    if((f = fopen(fileNameSample, "w")) == NULL)
    {
        printf("Error opening file %s.\n", fileNameSample);
        exit(0);
    }
    fileNameErr = (char *)calloc(strlen(fileNameKernel) + strlen(suffixErr) + 128, sizeof(char));
    sprintf(fileNameErr, "./outputs/%s_%s", fileNameKernel, suffixErr);
    printf("%s\n", fileNameErr);
    if((fErr = fopen(fileNameErr, "w")) == NULL)
    {
        printf("Error opening file %s.\n", fileNameErr);
        exit(0);
    }

    #ifdef FP   // FP number average
    // printf("FP number average\n");
    sum = xEnd.l - xStart.l;
    step = sum / testNumX;
    if(step == 0) {
        step = 1;
    }
    sumY = yEnd.l - yStart.l;
    stepY = sum / testNumY;
    if(stepY == 0) {
        stepY = 1;
    }

    printf("sum     : %lu 0x%lx\nstep    : %lu 0x%lx\ntestNum : %lu 0x%lx\n", sum, sum, step, step, testNumX * testNumY, testNumX * testNumY);
    // fprintf(f, "sum     : %lu 0x%lx\nstep    : %lu 0x%lx\ntestNum : %lu 0x%lx\n", sum, sum, step, step, testNumX * testNumY, testNumX * testNumY);
    for(i.l = yStart.l; i.l <= yEnd.l; i.l = i.l + stepY) {
        y = i.d;
        for(j.l = zStart.l; j.l <= zEnd.l; j.l = j.l + step) {
            x = j.d;

    #else   // Real number average
    // printf("Real number average\n");
    lenX = xEnd.d - xStart.d;
    lenY = yEnd.d - yStart.d;
    maxReUlp = 0;
    flag = 1;
    double stepXX0 = lenX / (double)testNumX;
    double stepXX1 = lenY / (double)testNumY;
    for(ii = 0; ii <= testNumY; ii++) {
        i.d = yStart.d + stepXX1 * ii;
        y = i.d;
        for(jj = 0; jj <= testNumX; jj++) {
            j.d = xStart.d + stepXX0 * jj;
            x = j.d;
    #endif
            computeResult2param(x, y, mpfrResult);
            computeOrcle2param(x, y, mpfrOrcle);
            #ifdef SINGLE
            reUlp = computeUlpDiffF(mpfrOrcle, mpfrResult);
            #else   // compute Double ULP as default
            reUlp = computeUlpDiff(mpfrOrcle, mpfrResult);
            #endif
            // orcle.d = mpfr_get_d(mpfrOrcle, MPFR_RNDN);
            // result.d = mpfr_get_d(mpfrResult, MPFR_RNDN);
            // fprintf(f, "%le\t%016lx\t%le\t%016lx\t%le\t%016lx\t%le\t%016lx\t%.16le\n", i.d, i.l, j.d, j.l, result.d, result.l, orcle.d, orcle.l, reUlp);
            // if(reUlp <= 0.5) {
            //     reUlp = 0;
            // }
            // if(isfinite(reUlp) == 0) {
            //     printf("happen to NaN or inf\n");
            //     exit(1);
            // }
            if(!isnormal(reUlp)) {
                // printf("x = %lg y = %lg reUlp = %.16e\n", x, y, reUlp);
                continue;;
            }
            fprintf(f, "%.16le ", reUlp);
            aveReUlp += reUlp;
            if (reUlp > maxReUlp) {
                flag = 0;
                maxInputY = i;
                maxInputX = j;
                maxReUlp = reUlp;
            }
        }
        fprintf(f, "\n");
    }
    // fprintf(f, "\n");
    aveReUlp = aveReUlp / (testNumX * testNumY);
    if(flag == 1) {
        printf("all error are 0!!\n");
    } else {
        printf("average ulp\tmax ulp\n");
        printf("%lg\t%lg\n", aveReUlp, maxReUlp);
        printf("\naveReUlp = %lg\nmaxInputX = 0x%016lx %lg, maxInputY = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX.l, maxInputX.d, maxInputY.l, maxInputY.d, maxReUlp);
        fprintf(fErr, "average ulp\tmax ulp\n");
        fprintf(fErr, "%lg\t%lg\n", aveReUlp, maxReUlp);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX = 0x%016lx %lg, maxInputY = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX.l, maxInputX.d, maxInputY.l, maxInputY.d, maxReUlp);
    }

    fclose(f);
    fclose(fErr);
    free(fileNameSample);
    free(fileNameErr);
}

int main(int argc, char *argv[]) {
    DL x0Start, x0End, x1Start, x1End;
    unsigned long int testNumX0, testNumX1;

    x0Start.d = 1;
    x0End.d = 10;
    x1Start.d = 0;
    x1End.d = 360;
    testNumX0 = TESTNUMX0;
    testNumX1 = TESTNUMX1;

    char *fileNameKernel;
    fileNameKernel = calloc(256, sizeof(char));
    if(argc == 8)
    {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        x1Start.d = strtod(argv[3], NULL);
        x1End.d = strtod(argv[4], NULL);
        testNumX0 = strtod(argv[5], NULL);
        testNumX1 = strtod(argv[6], NULL);
        strcpy(fileNameKernel, argv[7]);
    }
    else if(argc == 6)
    {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        x1Start.d = strtod(argv[3], NULL);
        x1End.d = strtod(argv[4], NULL);
        strcpy(fileNameKernel, argv[5]);
    }
    else if(argc == 4)
    {
        testNumX0 = strtod(argv[1], NULL);
        testNumX1 = strtod(argv[2], NULL);
        strcpy(fileNameKernel, argv[3]);
    }
    else
    {
        printf("Usage: ./test2param.out [x0Start x0End x1Start x1End testNumX0 testNumX1 fileNameKernel]\n");
        printf("Usage: if not correctly input:\n");
        printf("Usage: \tthe fixed inputs [%g %g %g %g %lu %lu] will be used\n", x0Start.d, x0End.d, x1Start.d, x1End.d, testNumX0, testNumX1);
    }
    printf("\n---------------------------------------------------start test2param\n");
    printf("Parameters: x0Start: %lg, x0End: %lg, x1Start: %lg, x1End: %lg, testNumX0 = %lu, testNumX1 = %lu, fileNameKernel: %s\n", x0Start.d, x0End.d, x1Start.d, x1End.d, testNumX0, testNumX1, fileNameKernel);

    test2param(x0Start, x0End, x1Start, x1End, testNumX0, testNumX1, fileNameKernel);

    return 0;
}