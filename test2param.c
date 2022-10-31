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

int computeOrcle2param(double x0, double x1, mpfr_t orcle) { return EXPRESSIONMPFR(x0, x1, orcle); }

int computeResult2param(double x0, double x1, mpfr_t mpfrResult)
{
    int status = 1;

    double result = EXPRESSIONMINE(x0, x1);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}

void test2param(DL x0Start, DL x0End, DL x1Start, DL x1End, unsigned long int testNumX0, unsigned long int testNumX1, const char *fileNameKernel)
{
    DL ii0, ii1, maxInputX0, maxInputX1;
    #ifdef DEBUG
    DL orcle, result;
    #endif
    int flag;
    double x0, x1, reUlp, aveReUlp, maxReUlp, lenX0, lenX1;
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

    // printf("Real number average\n");
    lenX0 = x0End.d - x0Start.d;
    lenX1 = x1End.d - x1Start.d;
    maxReUlp = 0;
    aveReUlp = 0;
    flag = 1;
    double stepX0 = lenX0 / (double)testNumX0;
    double stepX1 = lenX1 / (double)testNumX1;
    for(ii1.d = x1Start.d; ii1.d < x1End.d; ii1.d += stepX1)
    {
        x1 = ii1.d;
        for(ii0.d = x0Start.d; ii0.d < x0End.d; ii0.d += stepX0)
        {
            x0 = ii0.d;
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
            if (reUlp > maxReUlp)
            {
                flag = 0;
                maxInputX1 = ii1;
                maxInputX0 = ii0;
                maxReUlp = reUlp;
            }
        }
        // fprintf(f, "\n");
    }
    // fprintf(f, "\n");
    aveReUlp = aveReUlp / (testNumX0 * testNumX1);
    if(flag == 1)
    {
        printf("all error are 0!!\n");
    }
    else
    {
        printf("average ulp\tmax ulp\n");
        printf("%lg\t%lg\n", aveReUlp, maxReUlp);
        printf("\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxReUlp);
        fprintf(fErr, "average ulp\tmax ulp\n");
        fprintf(fErr, "%lg\t%lg\n", aveReUlp, maxReUlp);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX0 = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxReUlp);
    }

    fclose(f);
    fclose(fErr);
    free(fileNameSample);
    free(fileNameErr);
}

int main(int argc, char *argv[])
{
    DL x0Start, x0End, x1Start, x1End;
    unsigned long int testNumX0, testNumX1;

    x0Start.d = 1;
    x0End.d = 10;
    x1Start.d = 0;
    x1End.d = 360;
    testNumX0 = TESTNUMX;
    testNumX1 = TESTNUMY;

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
        printf("Usage: ./a.out [x0Start x0End x1Start x1End] testNumX0 testNumX1\n");
        printf("Usage: if not correctly input:\n");
        printf("Usage: \tthe fixed inputs in the program will be used\n");
    }
    printf("\n---------------------------------------------------start test2param\n");
    printf("Parameters: x0Start: %lg, x0End: %lg, x1Start: %lg, x1End: %lg, testNumX0 = %lu, testNumX1 = %lu, fileNameKernel: %s\n", x0Start.d, x0End.d, x1Start.d, x1End.d, testNumX0, testNumX1, fileNameKernel);

    test2param(x0Start, x0End, x1Start, x1End, testNumX0, testNumX1, fileNameKernel);

    return 0;
}