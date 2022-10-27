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

#define TESTNUMX 1024
#define TESTNUMY 1024
// #define FP
double EXPRESSIONMPFR(double, double, mpfr_t);
double EXPRESSIONMINE(double, double);

int computeOrcle2param(double x0, double x1, mpfr_t orcle) {
    return EXPRESSIONMPFR(x0, x1, orcle);
}

int computeResult2param(double x0, double x1, mpfr_t mpfrResult) {
    int status = 1;
    
    double result = EXPRESSIONMINE(x0, x1);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}

void test2param(DL x0Start, DL x0End, DL x1Start, DL x1End, unsigned long int testNumX0, unsigned long int testNumX1) {
    DL i, j, maxInputX0, maxInputX1;
    // DL orcle, result;
    int ii, jj, flag;
    double x0, x1, reUlp, aveReUlp, maxReUlp, lenX0, lenX1;
    #ifdef FP
    unsigned long int reUlpInt, step, stepY, sum, sumY, aveReUlpInt, maxReUlpInt;
    #endif
    char *directory = "./outputs";
    FILE *f;
    char *prefix = "";
    char *suffix = "sample.txt";
    char *fileName;
    FILE *fErr;
    char *prefixErr = "";
    char *suffixErr = "error.txt";
    char *fileNameErr;

    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // printf("test expression: %s\n", STR2(EXPRESSION));
    // printf("x0Start  : %lg 0x%016lx\nx0End    : %lg 0x%016lx\nx1Start  : %lg 0x%016lx\nx1End    : %lg 0x%016lx\n", x0Start.d, x0Start.l, x0End.d, x0End.l, x1Start.d, x1Start.l, x1End.d, x1End.l);
    // fprintf(f, "x0Start  : %lg 0x%016lx\nx0End    : %lg 0x%016lx\nx1Start  : %lg 0x%016lx\nx1End    : %lg 0x%016lx\n", x0Start.d, x0Start.l, x0End.d, x0End.l, x1Start.d, x1Start.l, x1End.d, x1End.l);
    // fprintf(f, "\nxInput\t\txInput (Hex)\t\tyInput\t\tyInput (Hex)\t\tresult\t\tresult (Hex)\t\torcle\t\torcle (Hex)\t\tulp error\n");
    // printf("testNum : %lu 0x%lx\n", testNumX0 * testNumX1, testNumX0 * testNumX1);
    fileName = (char *) malloc(strlen(prefix) + strlen(suffix) + 64);
    sprintf(fileName, "%s/%s%s_%d_%d_%d_%d_%lu_%lu_%s", directory, prefix, STR(EXPRESSION), (int)x0Start.d, (int)x0End.d, (int)x1Start.d, (int)x1End.d, testNumX0, testNumX1, suffix);
    printf("%s/%s%s_%d_%d_%d_%d_%lu_%lu_%s\n", directory, prefix, STR(EXPRESSION), (int)x0Start.d, (int)x0End.d, (int)x1Start.d, (int)x1End.d, testNumX0, testNumX1, suffix);
    if ((f = fopen(fileName, "w")) == NULL) { 
        printf("Error opening file %s.\n", fileName);
        exit(0);
    }
    fileNameErr = (char *) malloc(strlen(prefixErr) + strlen(suffixErr) + 64);
    sprintf(fileNameErr, "%s/%s%s_%d_%d_%d_%d_%lu_%lu_%s", directory, prefix, STR(EXPRESSION), (int)x0Start.d, (int)x0End.d, (int)x1Start.d, (int)x1End.d, testNumX0, testNumX1, suffixErr);
    printf("%s/%s%s_%d_%d_%d_%d_%lu_%lu_%s\n", directory, prefix, STR(EXPRESSION), (int)x0Start.d, (int)x0End.d, (int)x1Start.d, (int)x1End.d, testNumX0, testNumX1, suffixErr);
    if ((fErr = fopen(fileNameErr, "w")) == NULL)
    {
        printf("Error opening file %s.\n", fileNameErr);
        exit(0);
    }

    #ifdef FP   // FP number average
    // printf("FP number average\n");
    sum = x0End.l - x0Start.l;
    step = sum / testNumX0;
    if(step == 0) {
        step = 1;
    }
    sumY = x1End.l - x1Start.l;
    stepY = sum / testNumX1;
    if(stepY == 0) {
        stepY = 1;
    }

    printf("sum     : %lu 0x%lx\nstep    : %lu 0x%lx\ntestNum : %lu 0x%lx\n", sum, sum, step, step, testNumX0 * testNumX1, testNumX0 * testNumX1);
    // fprintf(f, "sum     : %lu 0x%lx\nstep    : %lu 0x%lx\ntestNum : %lu 0x%lx\n", sum, sum, step, step, testNumX0 * testNumX1, testNumX0 * testNumX1);
    for(i.l = x1Start.l; i.l <= x1End.l; i.l = i.l + stepY) {
        x1 = i.d;
        for(j.l = zStart.l; j.l <= zEnd.l; j.l = j.l + step) {
            x0 = j.d;

    #else   // Real number average
    // printf("Real number average\n");
    lenX0 = x0End.d - x0Start.d;
    lenX1 = x1End.d - x1Start.d;
    maxReUlp = 0;
    flag = 1;
    for(ii = 0; ii <= testNumX1; ii++) {
        i.d = x1Start.d + lenX1 / (double)testNumX1 * ii;
        x1 = i.d;
        for(jj = 0; jj <= testNumX0; jj++) {
            j.d = x0Start.d + lenX0 / (double)testNumX0 * jj;
            x0 = j.d;
    #endif
            // constraint: ((-2 * ((x1 * x1) * (x1 * x1))) + 2) >= x2;
            // if (x0 + x1 > 2) {
            //     continue;
            // }
            // floudas2
            // constraint0: (((((2 * ((x1 * x1) * (x1 * x1))) - ((8 * (x1 * x1)) * x1)) + ((8 * x1) * x1)) - x2) >= 0);
            // constraint1: (((((((4 * ((x1 * x1) * (x1 * x1))) - ((32 * (x1 * x1)) * x1)) + ((88 * x1) * x1)) - (96 * x1)) + 36) - x2) >= 0);
            // if(((((2 * ((x0 * x0) * (x0 * x0))) - ((8 * (x0 * x0)) * x0)) + ((8 * x0) * x0)) - x1) < 0) {
            //     reUlp = 0;
            //     fprintf(f, "%.16le ", reUlp);
            //     continue;
            // }
            // if(((((((4 * ((x0 * x0) * (x0 * x0))) - ((32 * (x0 * x0)) * x0)) + ((88 * x0) * x0)) - (96 * x0)) + 36) - x1) < 0) {
            //     reUlp = 0;
            //     fprintf(f, "%.16le ", reUlp);
            //     continue;
            // }
            computeResult2param(x0, x1, mpfrResult);
            computeOrcle2param(x0, x1, mpfrOrcle);
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
                // printf("x0 = %lg x1 = %lg reUlp = %.16e\n", x0, x1, reUlp);
                continue;
            }
            // fprintf(f, "%.16le ", reUlp);
            aveReUlp += reUlp;
            if (reUlp > maxReUlp) {
                flag = 0;
                maxInputX1 = i;
                maxInputX0 = j;
                maxReUlp = reUlp;
            }
        }
        // fprintf(f, "\n");
    }
    // fprintf(f, "\n");
    aveReUlp = aveReUlp / (testNumX0 * testNumX1);
    if(flag == 1) {
        printf("all error are 0!!\n");
    } else {
        printf("average ulp\tmax ulp\n");
        printf("%lg\t%lg\n", aveReUlp, maxReUlp);
        printf("\naveReUlp = %lg\nmaxInputX = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxReUlp);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxReUlp);
    }

    fclose(f);
    fclose(fErr);
}

int main(int argc, char *argv[]) {
    DL x0Start, x0End, x1Start, x1End;
    unsigned long int testNumX0, testNumX1;

    x0Start.d = 1;
    x0End.d = 10;
    x1Start.d = 0;
    x1End.d = 360;
    testNumX0 = TESTNUMX;
    testNumX1 = TESTNUMY;

    if(argc == 7) {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        x1Start.d = strtod(argv[3], NULL);
        x1End.d = strtod(argv[4], NULL);
        testNumX0 = strtod(argv[5], NULL);
        testNumX1 = strtod(argv[6], NULL);
    } else if(argc == 3) {
        testNumX0 = strtod(argv[1], NULL);
        testNumX1 = strtod(argv[2], NULL);
    } else {
        printf("Usage: ./a.out [x0Start x0End x1Start x1End] testNumX0 testNumX1\n");
        printf("Usage: if not correctly input:\n");
        printf("Usage: \tthe fixed inputs in the program will be used\n");
    }
    test2param(x0Start, x0End, x1Start, x1End, testNumX0, testNumX1);

    return 0;
}