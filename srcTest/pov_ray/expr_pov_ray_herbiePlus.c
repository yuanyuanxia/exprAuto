#include <math.h>
double expr_pov_ray_herbiePlus(double x1, double x2, double x3, double x4, double x5) {
	return (((x1 * cos(x2)) * cos(x3)) + (cos(x2) * x4)) + (x5 * (cos(x2) * sin(x3)));
}