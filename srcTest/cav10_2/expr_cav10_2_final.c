#include "mine.h"
double expr_cav10_2_final(double x) {
    double tmp;
	if (((x * x) - x) >= 0.0) {
		tmp = x / 10.0;
	} else {
        double result;
        if((0.05338 < x && x < 2.00062)) {
            result = fma(x, x, 2.0);
        } else if((2.44688 < x && x < 2.83436)) {
            result = fma(x, x, 2.0);
        } else if((3.73768 < x && x < 4.00484)) {
            result = fma(x, x, 2.0);
        } else if((5.47684 < x && x < 5.65892)) {
            result = fma(x, x, 2.0);
        } else if((7.86748 < x && x < 8.00748)) {
            result = fma(x, x, 2.0);
        } else {
            result = x * x + 2;
        }
        return result;
	}
	return tmp;
}
