#include <math.h>
double expr_squareRoot3Invalid_2_herbiePlus(double x) {
	double tmp;
	if (x < 0.0001) {
		tmp = fma(x, 0.5, 1.0);
	} else {
		tmp = sqrt((x + 1.0));
	}
	return tmp;
}