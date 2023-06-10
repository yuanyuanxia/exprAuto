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

double EXPRESSIONMPFR(double, double, mpfr_t);
double EXPRESSIONMINE(double, double);

int computeOrcle2param(double x0, double x1, mpfr_t orcle)
{
    return EXPRESSIONMPFR(x0, x1, orcle);
}

int computeResult2param(double x0, double x1, mpfr_t mpfrResult)
{
    int status = 1;

    double result = EXPRESSIONMINE(x0, x1);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}

void test2param(double x0, double x1)
{
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    computeResult2param(x0, x1, mpfrResult);
    computeOrcle2param(x0, x1, mpfrOrcle);

    double reUlp = computeUlpDiff(mpfrOrcle, mpfrResult);
    DL orcle, result;
    orcle.d = mpfr_get_d(mpfrOrcle, MPFR_RNDN);
    result.d = mpfr_get_d(mpfrResult, MPFR_RNDN);
    printf("%8lg %8lg %8lg\n", result.d, orcle.d, reUlp);
    printf("0x%016lx 0x%016lx %8lg\n", result.l, orcle.l, reUlp);

    // clear
    mpfr_clears(mpfrOrcle, mpfrResult, (mpfr_ptr) 0);
}

int main(int argc, char **argv)
{
    DL x0, x1;

    if (argc == 3)
    {
        x0.l = strtoul(argv[1], NULL, 16);
        x1.l = strtoul(argv[2], NULL, 16);
        printf("x0 = 0x%016lx\t", x0.l);
        printf("x1 = 0x%016lx\n", x1.l);
    }
    else
    {
        printf("Wrong number of arguments.\n");
        exit(EXIT_FAILURE);
    }
    test2param(x0.d, x1.d);
    return 0;
}
