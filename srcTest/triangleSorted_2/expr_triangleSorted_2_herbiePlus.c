#include <math.h>
double expr_triangleSorted_2_herbiePlus(double a, double b, double c) {
	double t_0 = b + (c - a);
	double t_1 = (c + (a - b)) * (a + (b + c));
	double tmp;
	if (a < b) {
		tmp = sqrt((((a + (b - c)) * t_1) * t_0)) / 4.0;
	} else {
		tmp = sqrt((t_0 * (t_1 * (b + (a - c))))) / 4.0;
	}
	return tmp;
}