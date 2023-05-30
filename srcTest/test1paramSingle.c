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

void test1param(double x0)
{
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    computeResult1param(x0, mpfrResult);
    computeOrcle1param(x0, mpfrOrcle);

    double reUlp = computeUlpDiff(mpfrOrcle, mpfrResult);
    DL orcle, result;
    orcle.d = mpfr_get_d(mpfrOrcle, MPFR_RNDN);
    result.d = mpfr_get_d(mpfrResult, MPFR_RNDN);
    printf("%8lg %8lg %8lg %8lg\n", x0, result.d, orcle.d, reUlp);

    // clear
    mpfr_clears(mpfrOrcle, mpfrResult, (mpfr_ptr) 0);
}

int main(int argc, char **argv)
{
    DL x0;

    if (argc == 2)
    {
        x0.l = strtoul(argv[1], NULL, 16);
        printf("Value = 0x%016lx\n", x0.l);
    }
    else
    {
        printf("Wrong number of arguments.\n");
        exit(EXIT_FAILURE);
    }
    test1param(x0.d);
    return 0;
}
