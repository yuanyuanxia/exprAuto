#include <math.h>
double expr_smartRoot_2_herbie(double c) {
	double t_0 = sqrt((12.25 + ((3.0 * c) * -4.0)));
	double t_1 = (-3.5 + t_0) / 6.0;
	double tmp_1;
	if ((12.25 - (3.0 * c)) > 10.0) {
		double tmp_2;
		if (3.5 > 0.0) {
			tmp_2 = (c * 2.0) / (-3.5 - t_0);
		} else if (3.5 < 0.0) {
			tmp_2 = t_1;
		} else {
			tmp_2 = t_1;
		}
		tmp_1 = tmp_2;
	} else {
		tmp_1 = t_1;
	}
	return tmp_1;
}