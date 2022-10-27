#include "common.h"

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

void test1param(DL x0Start, DL x0End, unsigned long int testNumX0)
{
    DL ii0, maxInputX0;
    #ifdef DEBUG
    DL orcle, result;
    #endif
    int i0, flag;
    double x0, reUlp, aveReUlp, maxReUlp, lenX0;
    #ifdef FP
    unsigned long int reUlpInt, stepX0, sum, aveReUlpInt, maxReUlpInt;
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
    // printf("x0Start  : %lg 0x%016lx\nx0End    : %lg 0x%016lx\n", x0Start.d, x0Start.l, x0End.d, x0End.l);
    // fprintf(f, "x0Start  : %lg 0x%016lx\nx0End    : %lg 0x%016lx\n", x0Start.d, x0Start.l, x0End.d, x0End.l);
    // fprintf(f, "\nxInput\t\txInput (Hex)\t\tresult\t\tresult (Hex)\t\torcle\t\torcle (Hex)\t\tulp error\n");
    // printf("testNum : %lu 0x%lx\n", testNumX0, testNumX0);
    fileName = (char *) malloc(strlen(prefix) + strlen(suffix) + 64);
    sprintf(fileName, "%s/%s%s_%d_%d_%lu%s", directory, prefix, STR(EXPRESSION), (int)x0Start.d, (int)x0End.d, testNumX0, suffix);
    printf("%s/%s%s_%d_%d_%lu%s\n", directory, prefix, STR(EXPRESSION), (int)x0Start.d, (int)x0End.d, testNumX0, suffix);
    if ((f = fopen(fileName, "w")) == NULL)
    {
        printf("Error opening file %s.\n", fileName);
        exit(0);
    }
    fileNameErr = (char *) malloc(strlen(prefixErr) + strlen(suffixErr) + 64);
    sprintf(fileNameErr, "%s/%s%s_%d_%d_%lu%s", directory, prefixErr, STR(EXPRESSION), (int)x0Start.d, (int)x0End.d, testNumX0, suffixErr);
    printf("%s/%s%s_%d_%d_%lu%s\n", directory, prefixErr, STR(EXPRESSION), (int)x0Start.d, (int)x0End.d, testNumX0, suffixErr);
    if ((fErr = fopen(fileNameErr, "w")) == NULL)
    {
        printf("Error opening file %s.\n", fileNameErr);
        exit(0);
    }

    #ifdef FP // FP number average
    // printf("FP number average\n");
    sum = x0End.l - x0Start.l;
    stepX0 = sum / testNumX0;
    if (stepX0 == 0)
    {
        stepX0 = 1;
    }

    printf("sum     : %lu 0x%lx\nstep    : %lu 0x%lx\ntestNum : %lu 0x%lx\n", sum, sum, stepX0, stepX0, testNumX0, testNumX0);
    // fprintf(f, "sum     : %lu 0x%lx\nstep    : %lu 0x%lx\ntestNum : %lu 0x%lx\n", sum, sum, stepX0, stepX0, testNumX0, testNumX0);
    for (ii0.l = zStart.l; ii0.l <= zEnd.l; ii0.l = ii0.l + stepX0)
    {
        x0 = ii0.d;

    #else // Real number average
    // printf("Real number average\n");
    lenX0 = x0End.d - x0Start.d;
    maxReUlp = 0;
    flag = 1;

    for (i0 = 0; i0 <= testNumX0; i0++)
    {
        ii0.d = x0Start.d + lenX0 / (double)testNumX0 * i0;
        x0 = ii0.d;
    #endif
        computeResult1param(x0, mpfrResult);
        computeOrcle1param(x0, mpfrOrcle);
        #ifdef SINGLE
        reUlp = computeUlpDiffF(mpfrOrcle, mpfrResult);
        #else // compute Double ULP as default
        reUlp = computeUlpDiff(mpfrOrcle, mpfrResult);
        #endif
        #ifdef DEBUG
        orcle.d = mpfr_get_d(mpfrOrcle, MPFR_RNDN);
        result.d = mpfr_get_d(mpfrResult, MPFR_RNDN);
        printf("%8lg %8lg %8lg %8lg\n", ii0.d, result.d, orcle.d, reUlp);
        #endif
        // if(reUlp <= 0.5) {
        //     reUlp = 0;
        // }
        // if(isfinite(reUlp) == 0) {
        //     printf("happen to NaN or inf\n");
        //     exit(1);
        // }
        fprintf(f, "%.16le\t%.16e\n", x0, reUlp);
        aveReUlp += reUlp;
        if (reUlp > maxReUlp)
        {
            flag = 0;
            maxInputX0 = ii0;
            maxReUlp = reUlp;
        }
    }
    // fprintf(f, "\n");
    aveReUlp = aveReUlp / (testNumX0);
    if (flag == 1)
    {
        printf("all error are 0!!\n");
    }
    else
    {
        printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxReUlp);
        fprintf(f, "\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxReUlp);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxReUlp);
    }

    fclose(f);
    fclose(fErr);
}

int main(int argc, char **argv)
{
    DL x0Start, x0End;
    unsigned long int testNumX0;

    x0Start.d = 1;
    x0End.d = 2;

    testNumX0 = TESTNUMX0;

    if (argc == 4)
    {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        testNumX0 = strtod(argv[3], NULL);
    }
    else if (argc == 3)
    {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
    }
    else if (argc == 2)
    {
        testNumX0 = strtod(argv[1], NULL);
    }
    else
    {
        printf("Usage: ./a.out [x0Start x0End testNumX0]\n");
        printf("Usage: if no correct input:\n");
        printf("Usage: \tthe fixed inputs [%g %g %lu] will be used\n", x0Start.d, x0End.d, testNumX0);
    }
    test1param(x0Start, x0End, testNumX0);

    return 0;
}
