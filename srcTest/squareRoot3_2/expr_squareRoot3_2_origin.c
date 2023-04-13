#include <math.h>
double expr_squareRoot3_2_origin(double x)
{
	double tmp;
	if (x < 1e-5) {
		tmp = 1.0 + (x * 0.5);
	} else {
		tmp = sqrt((x + 1.0));
	}
	return tmp;
}
