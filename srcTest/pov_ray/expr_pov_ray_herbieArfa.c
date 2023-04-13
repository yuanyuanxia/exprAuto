#include <math.h>
double expr_pov_ray_herbieArfa(double x1, double x2, double x3, double x4, double x5) {
	double result;
    if((-1 <= x1 && x1 <= 0) && (-3.141592653589793 <= x2 && x2 <= -1.8849555921538759) && (-3.141592653589793 <= x3 && x3 <= 1.2566370614359172) && (-1 <= x4 && x4 <= -0.6) && (-0.8 <= x5 && x5 <= 0.6000000000000001)) {
		return (((x1 * cos(x2)) * cos(x3)) + (cos(x2) * x4)) + ((cos(x2) * x5) * sin(x3));
    } else if((-1 <= x1 && x1 <= 0) && (-3.141592653589793 <= x2 && x2 <= -1.8849555921538759) && (-3.141592653589793 <= x3 && x3 <= 1.2566370614359172) && (0.40000000000000013 <= x4 && x4 <= 0.8) && (-0.8 <= x5 && x5 <= 0.6)) {
		return (((x1 * cos(x2)) * cos(x3)) + (cos(x2) * x4)) + ((cos(x2) * x5) * sin(x3));
    } else {
        result = cos(x2) * fma(x1, cos(x3), fma(x5, sin(x3), x4));
    }
    return result;
}