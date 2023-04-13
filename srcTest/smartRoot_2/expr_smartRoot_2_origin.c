#include <math.h>
double expr_smartRoot_2_origin(double c)
{
	double result;
	if (c < 0.75) {
		result = (c * 2.0) / (-3.5 - sqrt(((3.5 * 3.5) - ((3.0 * c) * 4.0))));
	} else {
		result = (-3.5 + sqrt(((3.5 * 3.5) - ((3.0 * c) * 4.0)))) / (3.0 * 2.0);
	}
	return result;
}
