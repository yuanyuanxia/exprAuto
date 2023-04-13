#include <math.h>
double expr_squareRoot3_2_herbieArfa(double x) {
	double tmp;
    double result;
	if (x < 1e-5) {
		tmp = 1.0 + 0.5 * x;
	} else {
		if((0.00015 < x && x < 2.01059)) {
			double tmp;
			if (x < 1e-5) {
				tmp = fma(x, 0.5, 1.0);
			} else {
				tmp = exp((0.5 * log1p(x)));
			}
			return tmp;
		} else if((2.98753 < x && x < 4.01163)) {
			double tmp;
			if (x < 1e-5) {
				tmp = fma(x, 0.5, 1.0);
			} else {
				tmp = exp((0.5 * log1p(x)));
			}
			return tmp;
		} else if((6.99244 < x && x < 8.0116)) {
			double tmp;
			if (x < 1e-5) {
				tmp = fma(x, 0.5, 1.0);
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