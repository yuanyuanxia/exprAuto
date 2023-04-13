#include <math.h>
double expr_squareRoot3_2_herbie(double x) {
	double tmp;
	if (x < 1e-5) {
		tmp = fma(x, 0.5, 1.0);
	} else {
		tmp = sqrt((x + 1.0));
	}
	return tmp;
}