#include <math.h>
double expr_pov_ray_herbie(double x1, double x2, double x3, double x4, double x5)
{
	double result = cos(x2) * fma(x1, cos(x3), fma(x5, sin(x3), x4));
	return result;
}
