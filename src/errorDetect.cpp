#include <cmath>
#include "errorDetect.hpp"

double computeUlpUnit(double x) {
    DL x0;
    x0.d = fabs(x);
    // printf("x0 = %le\n", x0.d);
    // printf("x0 = %016lx\n", x0.l);
    if(x0.d < pow(2, -1021)) {
        x0.d = pow(2, -1074);
    } else if(x0.d > (1 - pow(2, -53)) * pow(2, 1024)) {
        x0.d = pow(2,971);
    } else {
        x0.l = x0.l >> 52;
        // printf("1x0 = %016lx\n", x0.l);
        // x0.l = x0.l - 52;                // for these FP numbers whose exponent is lower than 52, the result will be zero wrongly.
        // printf("2x0 = %016lx\n", x0.l);
        x0.l = x0.l << 52;
        // printf("3x0 = %016lx\n", x0.l);
        x0.d = x0.d * pow(2, -52);
        // printf("4x0 = %016lx\n", x0.l);
        // printf("else: x0 = %le\n", x0.d);
    }
    return x0.d;
}

double computeError(mpfr::mpreal oracle, double x)
{
    // absolute error
    // double errorValue = fabs((oracle - x).toDouble());

    // relative error
    // double errorValue = fabs(((oracle - x) / oracle).toDouble());

    // ulp error
    auto unitUlp = computeUlpUnit(oracle.toDouble());
    auto errorValue = fabs((oracle - x).toDouble() / unitUlp);

    return errorValue;
}

double computeError(mpfr::mpreal oracle, mpfr::mpreal x)
{
    // absolute error
    // double errorValue = fabs((oracle - x).toDouble());

    // relative error
    // double errorValue = fabs(((oracle - x) / oracle).toDouble());

    // ulp error
    auto unitUlp = computeUlpUnit(oracle.toDouble());
    auto errorValue = fabs((oracle - x).toDouble() / unitUlp);

    return errorValue;
}