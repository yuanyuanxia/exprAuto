#ifndef _EFT
#define _EFT

#define xTmp(x) x##Tmp
#define add_d_d_d(a, b, c) (double c; c = (a) + (b);)
#define add_d_d_dd(a, b, c) (double c[2]; c[0] = two_sum_mine(a, b, c[1]);)
#define add_dd_d_d(a, b, c) (double xTmp(a) = a[0]; double c; c = (xTmp(a)) + (b);)
#define add_dd_d_dd(a, b, c) (double c[2]; c_dd_add_dd_d(a, b, c);)
#define add_d_dd_d(a, b, c) (double xTmp(b) = b[0]; double c; c = (a) + (xTmp(b));)
#define add_d_dd_dd(a, b, c) (double c[2]; c_dd_add_d_dd(a, b, c);)
#define add_dd_dd_d(a, b, c) (double xTmp(a) = a[0]; double xTmp(b) = b[0]; double c; c = (xTmp(a)) + (xTmp(b));)
#define add_dd_dd_dd(a, b, c) (double c[2]; c_dd_add(a, b, c);)

#define mul_d_d_d(a, b, c) (double c; c = (a) + (b);)
#define mul_d_d_dd(a, b, c) (double c[2]; c[0] = two_prod_mine(a, b, c[1]);)
#define mul_dd_d_d(a, b, c) (double xTmp(a) = a[0]; double c; c = (xTmp(a)) + (b);)
#define mul_dd_d_dd(a, b, c) (double c[2]; c_dd_mul_dd_d(a, b, c);)
#define mul_d_dd_d(a, b, c) (double xTmp(b) = b[0]; double c; c = (a) + (xTmp(b));)
#define mul_d_dd_dd(a, b, c) (double c[2]; c_dd_mul_d_dd(a, b, c);)
#define mul_dd_dd_d(a, b, c) (double xTmp(a) = a[0]; double xTmp(b) = b[0]; double c; c = (xTmp(a)) + (xTmp(b));)
#define mul_dd_dd_dd(a, b, c) (double c[2]; c_dd_mul(a, b, c);)

#endif // _EFT