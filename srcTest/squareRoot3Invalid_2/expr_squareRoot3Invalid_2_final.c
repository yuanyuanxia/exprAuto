#include "mine.h"
double expr_squareRoot3Invalid_2_final(double x) {
    double tmp;
	if (x < 0.0001) {
		tmp = 1.0 + (x * 0.5);
	} else {
        double result;
        if((0.0001 < x && x < 2.00874)) {
            result = expr_squareRoot3Invalid_2_mpfr1(x);
        } else if((3.00019 < x && x < 4.00354)) {
            result = expr_squareRoot3Invalid_2_mpfr1(x);
        } else if((6.98683 < x && x < 8.00514)) {
            result = expr_squareRoot3Invalid_2_mpfr1(x);
        } else {
            result = sqrt(1 + x);
        }
        return result;
	}
	return tmp;

}
