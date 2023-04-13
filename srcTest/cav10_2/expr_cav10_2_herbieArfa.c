#include <math.h>
double expr_cav10_2_herbieArfa(double x) {
	double tmp;
	if (((x * x) - x) >= 0.0) {
		tmp = x / 10.0;
	} else {
        if((0.05338 < x && x < 2.00062)) {
            double tmp;
			if (-x >= 0.0) {
				tmp = x * 0.1;
			} else {
				tmp = fma(x, x, 2.0);
			}
			return tmp;
        } else if((2.44688 < x && x < 2.83436)) {
            double tmp;
			if (-x >= 0.0) {
				tmp = x * 0.1;
			} else {
				tmp = fma(x, x, 2.0);
			}
			return tmp;
        } else if((3.73768 < x && x < 4.00484)) {
            double tmp;
			if (-x >= 0.0) {
				tmp = x * 0.1;
			} else {
				tmp = fma(x, x, 2.0);
			}
			return tmp;
        } else if((5.47684 < x && x < 5.65892)) {
            double tmp;
			if (-x >= 0.0) {
				tmp = x * 0.1;
			} else {
				tmp = fma(x, x, 2.0);
			}
			return tmp;
        } else if((7.86748 < x && x < 8.00748)) {
            double tmp;
			if (-x >= 0.0) {
				tmp = x * 0.1;
			} else {
				tmp = fma(x, x, 2.0);
			}
			return tmp;
        } else {
            double tmp;
			if (-x >= 0.0) {
				tmp = x * 0.1;
			} else {
				tmp = fma(x, x, 2.0);
			}
			return tmp;
        }
	}
	return tmp;
}