#ifndef _MULTITYPE
#define _MULTITYPE

#define xTmp(x) x##Tmp

// =========================================== ADD =======================================
#define add_d_d_d(a, b, c) double c = (a) + (b);
#define add_ld_d_d(a, b, c) long double xTmp(c) = (a) + (b); double c = xTmp(c);
#define add_dd_d_d(a, b, c) double xTmp(c)[2]; c_dd_add_dd_d(a, b, xTmp(c)); double c = xTmp(c)[0];
#define add_d_dd_d(a, b, c) double xTmp(c)[2]; c_dd_add_d_dd(a, b, xTmp(c)); double c = xTmp(c)[0];
#define add_ld_dd_d(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_add(xTmp(a), b, xTmp(c)); c = xTmp(c)[0]; // check c = ((a) + (b[0])) + b[1];
#define add_dd_dd_d(a, b, c) double xTmp(c)[2]; c_dd_add(a, b, xTmp(c)); double c; c = xTmp(c)[0];
#define add_d_ld_d(a, b, c) double c = (a) + (b);
#define add_ld_ld_d(a, b, c) double c = (a) + (b);
#define add_dd_ld_d(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double xTmp(c)[2]; c_dd_add(a, xTmp(b), xTmp(c)); c = xTmp(c)[0]; // check double c = ((a[0]) + (b[0])) + a[1];

#define add_d_d_dd(a, b, c) double c[2]; c[0] = two_add_mine(a, b, &(c[1])); // check c[0] = (a) + (b)
#define add_ld_d_dd(a, b, c) long double xTmp(c) = (a) + (b); double c[2]; c[0] = xTmp(c); c[1] = xTmp(c) - c[0];
#define add_dd_d_dd(a, b, c) double c[2]; c_dd_add_dd_d(a, b, c);
#define add_d_dd_dd(a, b, c) double c[2]; c_dd_add_d_dd(a, b, c);
#define add_ld_dd_dd(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double c[2]; c_dd_add(xTmp(a), b, c);
#define add_dd_dd_dd(a, b, c) double c[2]; c_dd_add(a, b, c);
#define add_d_ld_dd(a, b, c) long double xTmp(c) = (a) + (b); double c[2]; c[0] = xTmp(c); c[1] = c - xTmp(c)[0];
#define add_ld_ld_dd(a, b, c) long double xTmp(c) = (a) + (b); double c[2]; c[0] = xTmp(c); c[1] = c - xTmp(c)[0];
#define add_dd_ld_dd(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double c[2]; c_dd_add(a, xTmp(b), c);

#define add_d_d_ld(a, b, c) long double c = (a) + (b);
#define add_ld_d_ld(a, b, c) long double c = (a) + (b);
#define add_dd_d_ld(a, b, c) double xTmp(c)[2]; c_dd_add_dd_d(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define add_d_dd_ld(a, b, c) double xTmp(c)[2]; c_dd_add_d_dd(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define add_dd_dd_ld(a, b, c) double xTmp(c)[2]; c_dd_add(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define add_ld_dd_ld(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_add(xTmp(a), b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define add_d_ld_ld(a, b, c) long double c; c = (a) + (b);
#define add_ld_ld_ld(a, b, c) long double c; c = (a) + (b);
#define add_dd_ld_ld(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double xTmp(c)[2]; c_dd_add(a, xTmp(b), xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];

// =========================================== SUB =======================================
#define sub_d_d_d(a, b, c) double c = (a) - (b);
#define sub_ld_d_d(a, b, c) long double xTmp(c) = (a) - (b); double c = xTmp(c);
#define sub_dd_d_d(a, b, c) double xTmp(c)[2]; c_dd_sub_dd_d(a, b, xTmp(c)); double c = xTmp(c)[0];
#define sub_d_dd_d(a, b, c) double xTmp(c)[2]; c_dd_sub_d_dd(a, b, xTmp(c)); double c = xTmp(c)[0];
#define sub_ld_dd_d(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_sub(xTmp(a), b, xTmp(c)); c = xTmp(c)[0]; // check c = ((a) - (b[0])) - b[1];
#define sub_dd_dd_d(a, b, c) double xTmp(c)[2]; c_dd_sub(a, b, xTmp(c)); double c; c = xTmp(c)[0];
#define sub_d_ld_d(a, b, c) double c = (a) - (b);
#define sub_ld_ld_d(a, b, c) double c = (a) - (b);
#define sub_dd_ld_d(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double xTmp(c)[2]; c_dd_sub(a, xTmp(b), xTmp(c)); c = xTmp(c)[0]; // check double c = ((a[0]) - (b[0])) - a[1];

#define sub_d_d_dd(a, b, c) double c[2]; c[0] = two_sub_mine(a, b, &(c[1])); // check c[0] = (a) - (b)
#define sub_ld_d_dd(a, b, c) long double xTmp(c) = (a) - (b); double c[2]; c[0] = xTmp(c); c[1] = xTmp(c) - c[0];
#define sub_dd_d_dd(a, b, c) double c[2]; c_dd_sub_dd_d(a, b, c);
#define sub_d_dd_dd(a, b, c) double c[2]; c_dd_sub_d_dd(a, b, c);
#define sub_ld_dd_dd(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double c[2]; c_dd_sub(xTmp(a), b, c);
#define sub_dd_dd_dd(a, b, c) double c[2]; c_dd_sub(a, b, c);
#define sub_d_ld_dd(a, b, c) long double xTmp(c) = (a) - (b); double c[2]; c[0] = xTmp(c); c[1] = c - xTmp(c)[0];
#define sub_ld_ld_dd(a, b, c) long double xTmp(c) = (a) - (b); double c[2]; c[0] = xTmp(c); c[1] = c - xTmp(c)[0];
#define sub_dd_ld_dd(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double c[2]; c_dd_sub(a, xTmp(b), c);

#define sub_d_d_ld(a, b, c) long double c = (a) - (b);
#define sub_ld_d_ld(a, b, c) long double c = (a) - (b);
#define sub_dd_d_ld(a, b, c) double xTmp(c)[2]; c_dd_sub_dd_d(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define sub_d_dd_ld(a, b, c) double xTmp(c)[2]; c_dd_sub_d_dd(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define sub_dd_dd_ld(a, b, c) double xTmp(c)[2]; c_dd_sub(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define sub_ld_dd_ld(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_sub(xTmp(a), b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define sub_d_ld_ld(a, b, c) long double c; c = (a) - (b);
#define sub_ld_ld_ld(a, b, c) long double c; c = (a) - (b);
#define sub_dd_ld_ld(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double xTmp(c)[2]; c_dd_sub(a, xTmp(b), xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];

// =========================================== MUL =======================================
#define mul_d_d_d(a, b, c) double c = (a) * (b);
#define mul_ld_d_d(a, b, c) long double xTmp(c) = (a) * (b); double c = xTmp(c);
#define mul_dd_d_d(a, b, c) double xTmp(c)[2]; c_dd_mul_dd_d(a, b, xTmp(c)); double c = xTmp(c)[0];
#define mul_d_dd_d(a, b, c) double xTmp(c)[2]; c_dd_mul_d_dd(a, b, xTmp(c)); double c = xTmp(c)[0];
#define mul_ld_dd_d(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_mul(xTmp(a), b, xTmp(c)); c = xTmp(c)[0]; // TODO: check c = ((a) + (b[0])) + b[1];
#define mul_dd_dd_d(a, b, c) double xTmp(c)[2]; c_dd_mul(a, b, xTmp(c)); double c; c = xTmp(c)[0];
#define mul_d_ld_d(a, b, c) double c = (a) * (b);
#define mul_ld_ld_d(a, b, c) double c = (a) * (b);
#define mul_dd_ld_d(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double xTmp(c)[2]; c_dd_mul(a, xTmp(b), xTmp(c)); c = xTmp(c)[0]; // TODO: check double c = ((a[0]) + (b)) + a[1];

#define mul_d_d_dd(a, b, c) double c[2]; c[0] = two_mul_mine(a, b, &(c[1])); // check c[0] = (a) * (b)
#define mul_ld_d_dd(a, b, c) long double xTmp(c) = (a) * (b); double c[2]; c[0] = xTmp(c); c[1] = xTmp(c) - c[0];
#define mul_dd_d_dd(a, b, c) double c[2]; c_dd_mul_dd_d(a, b, c);
#define mul_d_dd_dd(a, b, c) double c[2]; c_dd_mul_d_dd(a, b, c);
#define mul_ld_dd_dd(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double c[2]; c_dd_mul(xTmp(a), b, c);
#define mul_dd_dd_dd(a, b, c) double c[2]; c_dd_mul(a, b, c);
#define mul_d_ld_dd(a, b, c) long double xTmp(c) = (a) * (b); double c[2]; c[0] = xTmp(c); c[1] = c - xTmp(c)[0];
#define mul_ld_ld_dd(a, b, c) long double xTmp(c) = (a) * (b); double c[2]; c[0] = xTmp(c); c[1] = c - xTmp(c)[0];
#define mul_dd_ld_dd(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double c[2]; c_dd_mul(a, xTmp(b), c);

#define mul_d_d_ld(a, b, c) long double c = (a) * (b);
#define mul_ld_d_ld(a, b, c) long double c = (a) * (b);
#define mul_dd_d_ld(a, b, c) double xTmp(c)[2]; c_dd_mul_dd_d(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define mul_d_dd_ld(a, b, c) double xTmp(c)[2]; c_dd_mul_d_dd(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define mul_dd_dd_ld(a, b, c) double xTmp(c)[2]; c_dd_mul(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define mul_ld_dd_ld(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_mul(xTmp(a), b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define mul_d_ld_ld(a, b, c) long double c; c = (a) * (b);
#define mul_ld_ld_ld(a, b, c) long double c; c = (a) * (b);
#define mul_dd_ld_ld(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double xTmp(c)[2]; c_dd_mul(a, xTmp(b), xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];

// =========================================== DIV =======================================
#define div_d_d_d(a, b, c) double c = (a) / (b);
#define div_ld_d_d(a, b, c) long double xTmp(c) = (a) / (b); double c = xTmp(c);
#define div_dd_d_d(a, b, c) double xTmp(c)[2]; c_dd_div_dd_d(a, b, xTmp(c)); double c = xTmp(c)[0];
#define div_d_dd_d(a, b, c) double xTmp(c)[2]; c_dd_div_d_dd(a, b, xTmp(c)); double c = xTmp(c)[0];
#define div_ld_dd_d(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_div(xTmp(a), b, xTmp(c)); c = xTmp(c)[0]; // TODO: check c = ((a) + (b[0])) + b[1];
#define div_dd_dd_d(a, b, c) double xTmp(c)[2]; c_dd_div(a, b, xTmp(c)); double c; c = xTmp(c)[0];
#define div_d_ld_d(a, b, c) double c = (a) / (b);
#define div_ld_ld_d(a, b, c) double c = (a) / (b);
#define div_dd_ld_d(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double xTmp(c)[2]; c_dd_div(a, xTmp(b), xTmp(c)); c = xTmp(c)[0]; // TODO: check double c = ((a[0]) + (b)) + a[1];

#define div_d_d_dd(a, b, c) double c[2]; c[0] = div_mine(a, b, &(c[1])); // check c[0] = (a) / (b)
#define div_ld_d_dd(a, b, c) long double xTmp(c) = (a) / (b); double c[2]; c[0] = xTmp(c); c[1] = xTmp(c) - c[0];
#define div_dd_d_dd(a, b, c) double c[2]; c_dd_div_dd_d(a, b, c);
#define div_d_dd_dd(a, b, c) double c[2]; c_dd_div_d_dd(a, b, c);
#define div_ld_dd_dd(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double c[2]; c_dd_div(xTmp(a), b, c);
#define div_dd_dd_dd(a, b, c) double c[2]; c_dd_div(a, b, c);
#define div_d_ld_dd(a, b, c) long double xTmp(c) = (a) / (b); double c[2]; c[0] = xTmp(c); c[1] = c - xTmp(c)[0];
#define div_ld_ld_dd(a, b, c) long double xTmp(c) = (a) / (b); double c[2]; c[0] = xTmp(c); c[1] = c - xTmp(c)[0];
#define div_dd_ld_dd(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double c[2]; c_dd_div(a, xTmp(b), c);

#define div_d_d_ld(a, b, c) long double c = (a) / (b);
#define div_ld_d_ld(a, b, c) long double c = (a) / (b);
#define div_dd_d_ld(a, b, c) double xTmp(c)[2]; c_dd_div_dd_d(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define div_d_dd_ld(a, b, c) double xTmp(c)[2]; c_dd_div_d_dd(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define div_dd_dd_ld(a, b, c) double xTmp(c)[2]; c_dd_div(a, b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define div_ld_dd_ld(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_div(xTmp(a), b, xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];
#define div_d_ld_ld(a, b, c) long double c; c = (a) / (b);
#define div_ld_ld_ld(a, b, c) long double c; c = (a) / (b);
#define div_dd_ld_ld(a, b, c) double xTmp(b)[2]; xTmp(b)[0] = b; xTmp(b)[1] = b - xTmp(b)[0]; double xTmp(c)[2]; c_dd_div(a, xTmp(b), xTmp(c)); long double c; c = xTmp(c)[0]; c += xTmp(c)[1];

#endif // _MULTITYPE
