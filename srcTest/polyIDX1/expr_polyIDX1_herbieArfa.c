#include <math.h>
double expr_polyIDX1_herbieArfa(double x1, double x2, double x3, double x4) {
	double result;
    if((3.6 < x1 && x1 < 5.4) && (1.85 < x2 && x2 < 4.65) && (5.2 < x3 && x3 < 7.6) && (3.05 < x4 && x4 < 3.5)) {
        double t_0 = (x2 * -0.8942341911807864) + ((x3 * 0.40478255170032273) + (x1 * 0.052100253064028955));
        double t_1 = pow(t_0, 2.0);
        result = pow(fma(0.45123431334216657, x2, fma(0.18424469636215, x1, fma(-0.8079426454076445, x3, (-0.4507112653440095 * x4)))), 3.0) + ((pow(x4, 3.0) * 0.8246257919139798) - ((((0.8793692521675407 * t_0) + (t_0 * 1.7587385043350814)) * pow(x4, 2.0)) + (pow(t_0, 3.0) + (x4 * ((-1.8754938039540847 * t_1) + (t_1 * -0.9377469019770424))))));
    } else {
        result = ((((((((0.18424469636215 * x1) + (0.45123431334216657 * x2)) + (-(0.8079426454076445) * x3)) + (-(0.4507112653440095) * x4)) * ((((0.18424469636215 * x1) + (0.45123431334216657 * x2)) + (-(0.8079426454076445) * x3)) + (-(0.4507112653440095) * x4))) * ((((0.18424469636215 * x1) + (0.45123431334216657 * x2)) + (-(0.8079426454076445) * x3)) - (0.4507112653440095 * x4))) - (((((((0.052100253064028955 * x1) + (-(0.8942341911807864) * x2)) + (0.40478255170032273 * x3)) * (((0.052100253064028955 * x1) + (-(0.8942341911807864) * x2)) + (0.40478255170032273 * x3))) + ((((0.052100253064028955 * x1) + (-(0.8942341911807864) * x2)) + (0.40478255170032273 * x3)) * (-(0.9377469019770424) * x4))) + (((((0.052100253064028955 * x1) + (-(0.8942341911807864) * x2)) + (0.40478255170032273 * x3)) * (-(0.9377469019770424) * x4)) + ((-(0.9377469019770424) * x4) * (-(0.9377469019770424) * x4)))) * (((0.052100253064028955 * x1) + (-(0.8942341911807864) * x2)) + (0.40478255170032273 * x3)))) - (((((((0.052100253064028955 * x1) - (0.8942341911807864 * x2)) + (0.40478255170032273 * x3)) * (((0.052100253064028955 * x1) + (-(0.8942341911807864) * x2)) + (0.40478255170032273 * x3))) + ((((0.052100253064028955 * x1) + (-(0.8942341911807864) * x2)) + (0.40478255170032273 * x3)) * (-(0.9377469019770424) * x4))) + ((((x3 * 0.40478255170032273) + ((-(0.8942341911807864) * x2) + (x1 * 0.052100253064028955))) * (-(0.9377469019770424) * x4)) + ((-(0.9377469019770424) * x4) * (-(0.9377469019770424) * x4)))) * (-(0.9377469019770424) * x4)));
    }
    return result;
}