#include <math.h>
double expr_triangleSorted_2_origin(double a, double b, double c) {
	double tmp;
	if (a < b) {
		tmp = sqrt(((((c + (b + a)) * (a - (c - b))) * (a + (c - b))) * (c + (b - a)))) / 4.0;
	} else {
		tmp = sqrt(((((c + (a + b)) * (b - (c - a))) * (b + (c - a))) * (c + (a - b)))) / 4.0;
	}
	return tmp;
}
