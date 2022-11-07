#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <mpfr.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpfr.h>
#define PRECISION 128

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

#define _EXP_BITS 11
#define SIGN_BIT 0x8000000000000000
#define EXP_BIT 0x7FF0000000000000
#define TAIL_BIT 0x000fffffffffffff
typedef unsigned long _TYPE;  // 8 bytes
typedef double DATATYPE;

// for data generation
typedef struct
{
    _TYPE m_nFraction : sizeof(_TYPE) * 8 - _EXP_BITS - 1;
    _TYPE m_nExp : _EXP_BITS;
    _TYPE m_nSign : 1;
} _DATA;

#endif
