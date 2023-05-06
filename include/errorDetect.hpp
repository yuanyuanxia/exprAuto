#ifndef _ERRORDETECT
#define _ERRORDETECT

#include "mpreal.h"

typedef union {
    double d;
    unsigned long int l;
} DL;

typedef union {
    float f;
    unsigned int l;
} FS;

double computeUlpUnit(double x);

double computeError(mpfr::mpreal oracle, double x);

#endif