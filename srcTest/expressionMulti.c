#include <math.h>

#include "multi.h"

// example: sin(x) + cos(y)
double example_mpfr(double x, double y, mpfr_t orcle)
{
    int status = 0;
    mpfr_t mp1, mp2, mp3, mp4, mp5;
    mpfr_init2(mp1, 128);
    mpfr_init2(mp2, 128);
    mpfr_init2(mp3, 128);
    mpfr_init2(mp4, 128);
    mpfr_init2(mp5, 128);
    mpfr_set_d(mp1, x, MPFR_RNDN);
    mpfr_sin(mp2, mp1, MPFR_RNDN);
    mpfr_set_d(mp3, y, MPFR_RNDN);
    mpfr_cos(mp4, mp3, MPFR_RNDN);
    mpfr_add(mp5, mp2, mp4, MPFR_RNDN);

    mpfr_set(orcle, mp5, MPFR_RNDN);
    mpfr_clear(mp1);
    mpfr_clear(mp2);
    mpfr_clear(mp3);
    mpfr_clear(mp4);
    mpfr_clear(mp5);
    return status;
}

double example_origin(double x, double y)
{
    double result = sin(x) + cos(y);
    return result;
}