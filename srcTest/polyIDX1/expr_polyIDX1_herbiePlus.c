#include <math.h>
double expr_polyIDX1_herbiePlus(double x1, double x2, double x3, double x4) {
	double t_0 = (((x1 * 0.052100253064028955) + (x2 * -0.8942341911807864)) + (x3 * 0.40478255170032273)) + (x4 * -0.9377469019770424);
	double t_1 = (-0.4507112653440095 * x4) + ((0.18424469636215 * x1) + (0.45123431334216657 * x2));
	double t_2 = pow(t_1, 2.0);
	return ((((-0.8079426454076445 * t_2) + (t_2 * -1.615885290815289)) * x3) + ((((t_1 * 1.3055426365366054) + (t_1 * 0.6527713182683027)) * pow(x3, 2.0)) + (pow(t_1, 3.0) + (-0.527401785727928 * pow(x3, 3.0))))) + (t_0 * (t_0 * ((((x1 * -0.052100253064028955) + (x2 * 0.8942341911807864)) + (x3 * -0.40478255170032273)) + (x4 * 0.9377469019770424))));
}