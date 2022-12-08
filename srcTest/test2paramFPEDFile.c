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

double EXPRESSIONMPFR(double, double, mpfr_t);
double EXPRESSIONMINE(double, double);

int computeOrcle2param(double x0, double x1, mpfr_t orcle) { return EXPRESSIONMPFR(x0, x1, orcle); }

int computeResult2param(double x0, double x1, mpfr_t mpfrResult) {
    int status = 1;

    double result = EXPRESSIONMINE(x0, x1);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}

void test2param(unsigned long int testNumX0, unsigned long int testNumX1, const char *fileNameKernel) {
    DL maxInputX0, maxInputX1;
    // DL orcle, result;
    unsigned long int ii, jj;
    int flag;
    double x0, x1, reUlp, aveReUlp, maxReUlp;

    FILE *inputFileX0;
    char *inputFileNameX0 = "./inputDataX0.txt";
    FILE *inputFileX1;
    char *inputFileNameX1 = "./inputDataX1.txt";
    char *directory = "./outputs";
    FILE *f;
    char *suffix = "sample.txt";
    char *fileNameSample;
    FILE *fErr;
    char *suffixErr = "error.txt";
    char *fileNameErr;

    if((inputFileX0 = fopen(inputFileNameX0, "r")) == NULL)
    {
        printf("Error opening file %s.\n", inputFileNameX0);
        exit(0);
    }
    if((inputFileX1 = fopen(inputFileNameX1, "r")) == NULL)
    {
        printf("Error opening file %s.\n", inputFileNameX1);
        exit(0);
    }

    fileNameSample = (char *)calloc(strlen(fileNameKernel) + strlen(suffix) + 128, sizeof(char));
    sprintf(fileNameSample, "./%s/%s_%s", directory, fileNameKernel, suffix);
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

    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // printf("Real number average\n");
    maxReUlp = 0;
    aveReUlp = 0;
    flag = 1;

    double *s0, *s1;
    unsigned long input;
    s0 = (double *) calloc(testNumX0 + 1, sizeof(double));
    s1 = (double *) calloc(testNumX1 + 1, sizeof(double));

    // for dimension X0
    for(ii = 0; ii <= testNumX0; ii++)
    {
        fscanf(inputFileX0, "%lx\n", &input);
        s0[ii] = *((double *)(&input));
    }

    // for dimension X1
    for(ii = 0; ii <= testNumX1; ii++)
    {
        fscanf(inputFileX1, "%lx\n", &input);
        s1[ii] = *((double *)(&input));
    }

    for(ii = 0; ii <= testNumX1; ii++) {
        x1 = s1[ii];
        for(jj = 0; jj <= testNumX0; jj++) {
            x0 = s0[jj];
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
                continue;;
            }
            fprintf(f, "%.16le ", reUlp);
            aveReUlp += reUlp;
            if (reUlp > maxReUlp) {
                flag = 0;
                maxInputX1.d = x1;
                maxInputX0.d = x0;
                maxReUlp = reUlp;
            }
        }
        fprintf(f, "\n");
    }
    // fprintf(f, "\n");
    aveReUlp = aveReUlp / (testNumX0 * testNumX1);
    if(flag == 1) {
        printf("all error are 0!!\n");
    } else {
        printf("average ulp\tmax ulp\n");
        printf("%lg\t%lg\n", aveReUlp, maxReUlp);
        printf("\naveReUlp = %lg\nmaxInputX = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxReUlp);
        fprintf(fErr, "average ulp\tmax ulp\n");
        fprintf(fErr, "%lg\t%lg\n", aveReUlp, maxReUlp);
        fprintf(fErr, "\naveReUlp = %lg\nmaxInputX = 0x%016lx %lg, maxInputX1 = 0x%016lx %lg, maxReUlp = %lg\n", aveReUlp, maxInputX0.l, maxInputX0.d, maxInputX1.l, maxInputX1.d, maxReUlp);
    }

    fclose(inputFileX0);
    fclose(inputFileX1);
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

    test2param(testNumX0, testNumX1, fileNameKernel);

    return 0;
}