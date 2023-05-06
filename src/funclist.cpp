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