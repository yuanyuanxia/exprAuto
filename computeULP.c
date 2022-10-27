#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#include "common.h"

inline double computeUlpUnit(double x) {
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

double computeUlpDiff(mpfr_t correctValue, mpfr_t myValue) {
    double tmp, unitUlp, reUlp;
    mpfr_t mpfrDiff, mpfrReUlp;

    mpfr_inits2 (PRECISION, mpfrDiff, mpfrReUlp, (mpfr_ptr) 0);

    mpfr_sub(mpfrDiff, correctValue, myValue, MPFR_RNDN); 
    tmp = mpfr_get_d(correctValue, MPFR_RNDN);
    // mpfr_printf ("correctValue =  %.26RNe\n", correctValue);
    // mpfr_printf ("myValue      =  %.26RNe\n", myValue);
    // mpfr_printf ("mpfrDiff     =  %.26RNe\n", mpfrDiff);
    // printf("tmp = %le\n", tmp);
    unitUlp = computeUlpUnit(tmp);
    // printf("unitUlp = %le\n", unitUlp);
    mpfr_div_d(mpfrReUlp, mpfrDiff, unitUlp, MPFR_RNDN);
    reUlp = mpfr_get_d(mpfrReUlp, MPFR_RNDN);
    reUlp = fabs(reUlp);

    return reUlp;
}

inline double computeUlpUnitF(double x) {
    DL x0;
    x0.d = fabs(x);
    // printf("x0 = %le\n", x0.d);
    // printf("x0 = %016lx\n", x0.l);
    if(x0.d < pow(2, -125)) {
        x0.d = pow(2, -149);
    } else if(x0.d > (1 - pow(2, -24)) * pow(2, 128)) {
        x0.d = pow(2, 104);
    } else {
        x0.l = x0.l >> 52;
        // printf("1x0 = %016lx\n", x0.l);
        // x0.l = x0.l - 52;                // for these FP numbers whose exponent is lower than 52, the result will be zero wrongly.
        // printf("2x0 = %016lx\n", x0.l);
        x0.l = x0.l << 52;
        // printf("3x0 = %016lx\n", x0.l);
        x0.d = x0.d * pow(2, -23);
        // printf("4x0 = %016lx\n", x0.l);
        // printf("else: x0 = %le\n", x0.d);
    }
    return x0.d;
}

double computeUlpDiffF(mpfr_t correctValue, mpfr_t myValue) {
    double tmp, unitUlp, reUlp;
    mpfr_t mpfrDiff, mpfrReUlp;

    mpfr_inits2 (PRECISION, mpfrDiff, mpfrReUlp, (mpfr_ptr) 0);

    mpfr_sub(mpfrDiff, correctValue, myValue, MPFR_RNDN); 
    tmp = mpfr_get_d(correctValue, MPFR_RNDN);
    // mpfr_printf ("correctValue =  %.26RNe\n", correctValue);
    // mpfr_printf ("myValue      =  %.26RNe\n", myValue);
    // mpfr_printf ("mpfrDiff     =  %.26RNe\n", mpfrDiff);
    // printf("tmp = %le\n", tmp);
    unitUlp = computeUlpUnitF(tmp);
    // printf("unitUlp = %le\n", unitUlp);
    mpfr_div_d(mpfrReUlp, mpfrDiff, unitUlp, MPFR_RNDN);
    reUlp = mpfr_get_d(mpfrReUlp, MPFR_RNDN);
    reUlp = fabs(reUlp);

    return reUlp;
}
