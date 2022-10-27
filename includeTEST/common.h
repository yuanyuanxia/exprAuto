#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <mpfr.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpfr.h>
#define PRECISION 120

typedef union {
    double d;
    unsigned long int l;
} DL;

typedef union {
    float f;
    unsigned int l;
} FS;

#define _STR(R)  #R
#define STR(R)  _STR(R)
#define GLUE(s,d) s ## d
#define ADDSUFFIX(func, suffix) GLUE(func, suffix)
#define ADDPREFIX(prefix, func) GLUE(prefix, func)

double computeUlpUnit(double x);

double computeUlpUnitF(double x);

double computeUlpDiff(mpfr_t correctValue, mpfr_t myValue);

double computeUlpDiffF(mpfr_t correctValue, mpfr_t myValue);

#endif
