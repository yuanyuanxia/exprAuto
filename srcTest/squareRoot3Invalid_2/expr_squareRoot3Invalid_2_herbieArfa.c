#include <math.h>
double expr_squareRoot3Invalid_2_herbieArfa(double x) {
	double tmp;
	if (x < 0.0001) {
		tmp = 1.0 + (x * 0.5);
	} else {
		double result;
		if((0.0001 < x && x < 2.00874)) {
			double tmp;
			if (x < 0.0001) {
				tmp = 1.0 + (x * 0.5);
			} else {
				tmp = exp((0.5 * log1p(x)));
			}
			return tmp;
		} else if((3.00019 < x && x < 4.00354)) {
			double tmp;
			if (x < 0.0001) {
				tmp = 1.0 + (x * 0.5);
			} else {
				tmp = exp((0.5 * log1p(x)));
			}
			return tmp;
		} else if((6.98683 < x && x < 8.00514)) {
			double tmp;
			if (x < 0.0001) {
				tmp = 1.0 + (x * 0.5);
			} else {
				tmp = exp((0.5 * log1p(x)));
			}
			return tmp;
		} else {
			result = sqrt(1 + x);
		}
		return result;
	}
	return tmp;
}