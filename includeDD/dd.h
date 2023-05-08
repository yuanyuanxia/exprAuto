#ifndef _MULTITYPE
#define _MULTITYPE

#define xTmp(x) x##Tmp

// TODO: 操作的实现精度取决于操作结果的类型以及变量类型。如果结果是dd类型，那实现必须按照dd的方法来。如果结果是double，那要看变量类型有无dd。如果有的话，就得使用dd算法，只不过最后要将结果舍入到double。
// 解释：这么做的原因在于，避免上一步的精度优化白做。如果上步采用的是dd，那其精度提升效果主要体现在新增的精度位。如果当前步骤舍去新增的精度位，那就相当于舍弃了上一步的精度优化效果。
// 解释：虽然这么做，感觉是存在一些性能上的牵连，但如果不这么做的话，那精度优化的意义就不大了。而且从优化的角度，也鼓励一条龙式的优化，这样可以尽量避免精度、性能浪费。
// 我的疑问就是：dd+double的话，究竟算是dd实现，还是double实现？dd+dd和ld+dd的精度实现有差异吗

// =========================================== ADD =======================================
// 先只改dd、double
#define add_d_d_d(a, b, c) double c = (a) + (b);
#define add_ld_d_d(a, b, c) long double xTmp(c) = (a) + (b); double c = xTmp(c);
#define add_dd_d_d(a, b, c) double xTmp(c)[2]; c_dd_add_dd_d(a, b, xTmp(c)); double c = xTmp(c)[0] + xTmp(c)[1];
#define add_d_dd_d(a, b, c) double xTmp(c)[2]; c_dd_add_d_dd(a, b, xTmp(c)); double c = xTmp(c)[0] + xTmp(c)[1];
#define add_ld_dd_d(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_add(xTmp(a), b, xTmp(c)); c = xTmp(c)[0]; // check c = ((a) + (b[0])) + b[1];
#define add_dd_dd_d(a, b, c) double xTmp(c)[2]; c_dd_add(a, b, xTmp(c)); double c; c = xTmp(c)[0] + xTmp(c)[1];
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
#define sub_dd_d_d(a, b, c) double xTmp(c)[2]; c_dd_sub_dd_d(a, b, xTmp(c)); double c = xTmp(c)[0] + xTmp(c)[1];
#define sub_d_dd_d(a, b, c) double xTmp(c)[2]; c_dd_sub_d_dd(a, b, xTmp(c)); double c = xTmp(c)[0] + xTmp(c)[1];
#define sub_ld_dd_d(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_sub(xTmp(a), b, xTmp(c)); c = xTmp(c)[0]; // check c = ((a) - (b[0])) - b[1];
#define sub_dd_dd_d(a, b, c) double xTmp(c)[2]; c_dd_sub(a, b, xTmp(c)); double c; c = xTmp(c)[0] + xTmp(c)[1];
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
#define mul_dd_d_d(a, b, c) double xTmp(c)[2]; c_dd_mul_dd_d(a, b, xTmp(c)); double c = xTmp(c)[0] + xTmp(c)[1];
#define mul_d_dd_d(a, b, c) double xTmp(c)[2]; c_dd_mul_d_dd(a, b, xTmp(c)); double c = xTmp(c)[0] + xTmp(c)[1];
#define mul_ld_dd_d(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_mul(xTmp(a), b, xTmp(c)); c = xTmp(c)[0]; // TODO: check c = ((a) + (b[0])) + b[1];
#define mul_dd_dd_d(a, b, c) double xTmp(c)[2]; c_dd_mul(a, b, xTmp(c)); double c; c = xTmp(c)[0] + xTmp(c)[1];
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
#define div_dd_d_d(a, b, c) double xTmp(c)[2]; c_dd_div_dd_d(a, b, xTmp(c)); double c = xTmp(c)[0] + xTmp(c)[1];
#define div_d_dd_d(a, b, c) double xTmp(c)[2]; c_dd_div_d_dd(a, b, xTmp(c)); double c = xTmp(c)[0] + xTmp(c)[1];
#define div_ld_dd_d(a, b, c) double xTmp(a)[2]; xTmp(a)[0] = a; xTmp(a)[1] = a - xTmp(a)[0]; double xTmp(c)[2]; c_dd_div(xTmp(a), b, xTmp(c)); c = xTmp(c)[0]; // TODO: check c = ((a) + (b[0])) + b[1];
#define div_dd_dd_d(a, b, c) double xTmp(c)[2]; c_dd_div(a, b, xTmp(c)); double c; c = xTmp(c)[0] + xTmp(c)[1];
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
