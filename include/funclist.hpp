#ifndef _FUNCLIST
#define _FUNCLIST
#include <math.h>
#include "mpreal.h"

using std::vector;

double mineadd(double x, double y);

double minesub(double x, double y);

double minemul(double x, double y);

double minediv(double x, double y);

double minesin(double x);

double minecos(double x);

double minetan(double x);

double mineexp(double x);

double minelog(double x);

double mineasin(double x);

double mineacos(double x);

double mineatan(double x);

double common_sin(vector<double> args);

double common_cos(vector<double> args);

double common_tan(vector<double> args);

mpfr::mpreal realadd(mpfr::mpreal x, mpfr::mpreal y);

mpfr::mpreal realsub(mpfr::mpreal x, mpfr::mpreal y);

mpfr::mpreal realmul(mpfr::mpreal x, mpfr::mpreal y);

mpfr::mpreal realdiv(mpfr::mpreal x, mpfr::mpreal y);

mpfr::mpreal real_tan(vector<mpfr::mpreal> args);

#endif