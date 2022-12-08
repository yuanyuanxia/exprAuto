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

void test1param(DL x0Start, DL x0End, unsigned long int testNumX0, const char* fileNameKernel)
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
    char *suffix = "sample.txt";
    char *fileNameSample;
    FILE *fErr;
    char *suffixErr = "error.txt";
    char *fileNameErr;

    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // printf("test expression: %s\n", STR2(EXPRESSION));
    // printf("x0Start  : %lg 0x%016lx\nx0End    : %lg 0x%016lx\n", x0Start.d, x0Start.l, x0End.d, x0End.l);
    // fprintf(f, "x0Start  : %lg 0x%016lx\nx0End    : %lg 0x%016lx\n", x0Start.d, x0Start.l, x0End.d, x0End.l);
    // fprintf(f, "\nxInput\t\txInput (Hex)\t\tresult\t\tresult (Hex)\t\torcle\t\torcle (Hex)\t\tulp error\n");
    // printf("testNum : %lu 0x%lx\n", testNumX0, testNumX0);
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
    aveReUlp = 0;
    flag = 1;
    double stepX0 = lenX0 / (double)testNumX0;
    for(ii0.d = x0Start.d; ii0.d < x0End.d; ii0.d += stepX0)
    {
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
        // fprintf(f, "%.16le\t%.16e\n", x0, reUlp);
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
        printf("average ulp\tmax ulp\n");
        printf("%lg\t%lg\n", aveReUlp, maxReUlp);
        printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxReUlp);
        fprintf(fErr, "average ulp\tmax ulp\n");
        fprintf(fErr, "%lg\t%lg\n", aveReUlp, maxReUlp);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxReUlp);
    }

    // clear
    fclose(f);
    fclose(fErr);
    free(fileNameErr);
    free(fileNameSample);
    mpfr_clears(mpfrOrcle, mpfrResult, (mpfr_ptr) 0);
}

int main(int argc, char **argv)
{
    DL x0Start, x0End;
    unsigned long int testNumX0;

    x0Start.d = 1;
    x0End.d = 2;
    testNumX0 = TESTNUMX0;

    char *fileNameKernel;
    fileNameKernel = calloc(256, sizeof(char));
    if (argc == 5)
    {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        testNumX0 = strtod(argv[3], NULL);
        strcpy(fileNameKernel, argv[4]);
    }
    else if (argc == 4)
    {
        x0Start.d = strtod(argv[1], NULL);
        x0End.d = strtod(argv[2], NULL);
        strcpy(fileNameKernel, argv[3]);
    }
    else if (argc == 3)
    {
        testNumX0 = strtod(argv[1], NULL);
        strcpy(fileNameKernel, argv[2]);
    }
    else
    {
        printf("Usage: ./test1param.exe [x0Start x0End testNumX0]\n");
        printf("Usage: if no correct input:\n");
        printf("Usage: \tthe fixed inputs [%g %g %lu] will be used\n", x0Start.d, x0End.d, testNumX0);
    }
    printf("\n---------------------------------------------------start test1param\n");
    printf("Parameters: x0Start: %lg, x0End: %lg, testNumX0 = %lu, fileNameKernel: %s\n", x0Start.d, x0End.d, testNumX0, fileNameKernel);
    test1param(x0Start, x0End, testNumX0, fileNameKernel);

    // clear
    free(fileNameKernel);
    return 0;
}
