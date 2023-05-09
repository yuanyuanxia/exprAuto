#include <qd/c_dd.h>
#include "basic.hpp"
#include "funclist.hpp"

double mineadd(double x, double y) {
    return x + y;
}

double minesub(double x, double y) {
    return x - y;
}

double minemul(double x, double y) {
    return x * y;
}

double minediv(double x, double y) {
    return x / y;
}

double minesin(double x) {
    return sin(x);
}

double minecos(double x) {
    return cos(x);
}

double minetan(double x) {
    return tan(x);
}

double mineexp(double x) {
    return exp(x);
}

double minelog(double x) {
    return log(x);
}

double mineasin(double x) {
    return asin(x);
}

double mineacos(double x) {
    return acos(x);
}

double mineatan(double x) {
    return atan(x);
}

double common_sin(vector<double> args) {
    return sin(args.at(0));
}

double common_cos(vector<double> args) {
    return cos(args.at(0));
}

double common_tan(vector<double> args) {
    return tan(args.at(0));
}

double common_pow(vector<double> args) {
    return pow(args.at(0), args.at(1));
}

mpfr::mpreal realadd(mpfr::mpreal x, mpfr::mpreal y)
{
    return x + y;
}

mpfr::mpreal realsub(mpfr::mpreal x, mpfr::mpreal y)
{
    return x - y;
}

mpfr::mpreal realmul(mpfr::mpreal x, mpfr::mpreal y)
{
    return x * y;
}

mpfr::mpreal realdiv(mpfr::mpreal x, mpfr::mpreal y)
{
    return x / y;
}

mpfr::mpreal real_tan(vector<mpfr::mpreal> args) {
    return tan(args.at(0));
}

mpfr::mpreal real_pow(vector<mpfr::mpreal> args) {
    return pow(args.at(0), args.at(1));
}

template<typename T>
mpfr::mpreal mineadd_d_dd(double x1, double *x2Ptr)
{
    double y[2];
    c_dd_add_d_dd(x1, x2Ptr, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal mineadd_dd_d(double *x1Ptr, double x2)
{
    double y[2];
    c_dd_add_dd_d(x1Ptr, x2, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal mineadd_dd_dd(double *x1Ptr, double *x2Ptr)
{
    double y[2];
    c_dd_add(x1Ptr, x2Ptr, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minesub_d_dd(double x1, double *x2Ptr)
{
    double y[2];
    c_dd_sub_d_dd(x1, x2Ptr, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minesub_dd_d(double *x1Ptr, double x2)
{
    double y[2];
    c_dd_sub_dd_d(x1Ptr, x2, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minesub_dd_dd(double *x1Ptr, double *x2Ptr)
{
    double y[2];
    c_dd_sub(x1Ptr, x2Ptr, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minemul_d_dd(double x1, double *x2Ptr)
{
    double y[2];
    c_dd_mul_d_dd(x1, x2Ptr, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minemul_dd_d(double *x1Ptr, double x2)
{
    double y[2];
    c_dd_mul_dd_d(x1Ptr, x2, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minemul_dd_dd(double *x1Ptr, double *x2Ptr)
{
    double y[2];
    c_dd_mul(x1Ptr, x2Ptr, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minediv_d_dd(double x1, double *x2Ptr)
{
    double y[2];
    c_dd_div_d_dd(x1, x2Ptr, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minediv_dd_d(double *x1Ptr, double x2)
{
    double y[2];
    c_dd_div_dd_d(x1Ptr, x2, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minediv_dd_dd(double *x1Ptr, double *x2Ptr)
{
    double y[2];
    c_dd_div(x1Ptr, x2Ptr, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minesin_dd(double *x)
{
    double y[2];
    c_dd_sin(x, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

template<typename T>
mpfr::mpreal minetan_dd(double *x)
{
    double y[2];
    c_dd_tan(x, y);
    T tmp;
    mpfr::mpreal result;
    tmp = y[0];
    result = tmp + y[1];
    return result;
}

// add
template mpfr::mpreal mineadd_d_dd<double>(double x, double *y);
template mpfr::mpreal mineadd_d_dd<mpfr::mpreal>(double x, double *y);
template mpfr::mpreal mineadd_dd_d<double>(double *x, double y);
template mpfr::mpreal mineadd_dd_d<mpfr::mpreal>(double *x, double y);
template mpfr::mpreal mineadd_dd_dd<double>(double *x, double *y);
template mpfr::mpreal mineadd_dd_dd<mpfr::mpreal>(double *x, double *y);

// sub
template mpfr::mpreal minesub_d_dd<double>(double x, double *y);
template mpfr::mpreal minesub_d_dd<mpfr::mpreal>(double x, double *y);
template mpfr::mpreal minesub_dd_d<double>(double *x, double y);
template mpfr::mpreal minesub_dd_d<mpfr::mpreal>(double *x, double y);
template mpfr::mpreal minesub_dd_dd<double>(double *x, double *y);
template mpfr::mpreal minesub_dd_dd<mpfr::mpreal>(double *x, double *y);

// mul
template mpfr::mpreal minemul_d_dd<double>(double x, double *y);
template mpfr::mpreal minemul_d_dd<mpfr::mpreal>(double x, double *y);
template mpfr::mpreal minemul_dd_d<double>(double *x, double y);
template mpfr::mpreal minemul_dd_d<mpfr::mpreal>(double *x, double y);
template mpfr::mpreal minemul_dd_dd<double>(double *x, double *y);
template mpfr::mpreal minemul_dd_dd<mpfr::mpreal>(double *x, double *y);

// div
template mpfr::mpreal minediv_d_dd<double>(double x, double *y);
template mpfr::mpreal minediv_d_dd<mpfr::mpreal>(double x, double *y);
template mpfr::mpreal minediv_dd_d<double>(double *x, double y);
template mpfr::mpreal minediv_dd_d<mpfr::mpreal>(double *x, double y);
template mpfr::mpreal minediv_dd_dd<double>(double *x, double *y);
template mpfr::mpreal minediv_dd_dd<mpfr::mpreal>(double *x, double *y);

// single math function
template mpfr::mpreal minesin_dd<double>(double *x);
template mpfr::mpreal minesin_dd<mpfr::mpreal>(double *x);
template mpfr::mpreal minetan_dd<double>(double *x);
template mpfr::mpreal minetan_dd<mpfr::mpreal>(double *x);