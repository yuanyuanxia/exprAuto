#include "mine.h"
double expr_squareRoot3_2_final(double x) {
    double tmp;
	if (x < 1e-5) {
		tmp = 1.0 + 0.5 * x;
	} else {
		double result;
        if((0.00015 < x && x < 2.01059)) {
            result = expr_squareRoot3_2_mpfr1(x);
        } else if((2.98753 < x && x < 4.01163)) {
            result = expr_squareRoot3_2_mpfr1(x);
        } else if((6.99244 < x && x < 8.0116)) {
            result = expr_squareRoot3_2_mpfr1(x);
        } else {
            result = sqrt(1 + x);
        }
        return result;
	}
	return tmp;
}
