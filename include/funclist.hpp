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

double common_pow(vector<double> args);

mpfr::mpreal realadd(mpfr::mpreal x, mpfr::mpreal y);

mpfr::mpreal realsub(mpfr::mpreal x, mpfr::mpreal y);

mpfr::mpreal realmul(mpfr::mpreal x, mpfr::mpreal y);

mpfr::mpreal realdiv(mpfr::mpreal x, mpfr::mpreal y);

mpfr::mpreal real_tan(vector<mpfr::mpreal> args);

mpfr::mpreal real_pow(vector<mpfr::mpreal> args);

template<typename T>
mpfr::mpreal mineadd_d_dd(double x, double *y);

template<typename T>
mpfr::mpreal mineadd_dd_d(double *x, double y);

template<typename T>
mpfr::mpreal mineadd_dd_dd(double *x, double *y);

template<typename T>
mpfr::mpreal minesub_d_dd(double x, double *y);

template<typename T>
mpfr::mpreal minesub_dd_d(double *x, double y);

template<typename T>
mpfr::mpreal minesub_dd_dd(double *x, double *y);

template<typename T>
mpfr::mpreal minemul_d_dd(double x, double *y);

template<typename T>
mpfr::mpreal minemul_dd_d(double *x, double y);

template<typename T>
mpfr::mpreal minemul_dd_dd(double *x, double *y);

template<typename T>
mpfr::mpreal minediv_d_dd(double x, double *y);

template<typename T>
mpfr::mpreal minediv_dd_d(double *x, double y);

template<typename T>
mpfr::mpreal minediv_dd_dd(double *x, double *y);

template<typename T>
mpfr::mpreal minesin_dd(double *x);

template<typename T>
mpfr::mpreal minetan_dd(double *x);

#endif