#include <math.h>

/* Computes fl(a+b) and err(a+b).  */
inline double two_sum_mine(double a, double b, double &err)
{
    double s = a + b;
    double bb = s - a;
    err = (a - (s - bb)) + (b - bb);
    return s;
}

/* Computes fl(a*b) and err(a*b). */
inline double two_prod_mine(double a, double b, double &err)
{
    double p = a * b;
    err = fma(a, b, -p);
    return p;
}