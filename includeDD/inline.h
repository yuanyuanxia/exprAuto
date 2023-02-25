#include <math.h>
#include <qd/c_dd.h>

/* Computes fl(a+b) and err(a+b).  */
static inline double two_add_mine(double a, double b, double *err)
{
    double s = a + b;
    double bb = s - a;
    *err = (a - (s - bb)) + (b - bb);
    return s;
}

/* Computes fl(a*b) and err(a*b). */
static inline double two_mul_mine(double a, double b, double *err)
{
    double p = a * b;
    *err = fma(a, b, -p);
    return p;
}

static inline double two_sub_mine(double a, double b, double *err)
{
    double s = a - b;
    double bb = s - a;
    *err = (a - (s - bb)) - (b + bb);
    return s;
}

static inline double quick_two_sum_mine(double a, double b, double *err)
{
    double s = a + b;
    *err = b - (s - a);
    return s;
}

static inline double div_mine(double a, double b, double *err)
{
    double q1, q2;
    double p1, p2;
    double s;

    q1 = a / b;

    /* Compute  a - q1 * b */
    p1 = two_mul_mine(q1, b, &p2);
    s = two_sub_mine(a, p1, err);
    *err -= p2;

    /* get next approximation */
    q2 = (s + *err) / b;

    s = quick_two_sum_mine(q1, q2, err);
    return s;
}

// pow = exp(b * log(a))
static inline void c_dd_pow_mine(double *a, double *b, double *result) {
    if(a[0] == 0 && a[1] == 0)
    {
        result[0] = 0;
        result[1] = 0;
        return;
    }
    double tmp1[2];
    double tmp2[2];
    c_dd_log(a, tmp1);
    c_dd_mul(tmp1, b, tmp2);
    c_dd_exp(tmp2, result);
}