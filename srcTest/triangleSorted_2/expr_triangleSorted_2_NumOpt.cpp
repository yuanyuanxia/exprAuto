#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <cfenv>

#include "backend/points.h"
#include "backend/self_math.h"
#include "backend/gamma.h"
#include "iRRAM/lib.h"

int iRRAM::MAXiterationnum = 30;
bool iRRAM::enableReiterate = true;
bool iRRAM::alwaysenableReiterate = true;

using namespace std;
using namespace iRRAM;

double expr_triangleSorted_2_CPP(double a, double b, double c)
{
	REAL a_real(a);
	REAL b_real(b);
	REAL c_real(c);
	double r;
	REAL r_real;

	if(a < b&&(-9.838516996763401e-208<=a)&&(a<=-4.1527009680941224e-210)) {
		r = sqrt(((((c + (b + a)) * (a - (c - b))) * (a + (c - b))) * (c + (b - a)))) / 4.0;
		return r;
	}

	if(a < b&&(-1.8472342742723307e-249<=a)&&(a<=-1.8472342742723307e-249)) {
		r = sqrt(((((c + (b + a)) * (a - (c - b))) * (a + (c - b))) * (c + (b - a)))) / 4.0;
		return r;
	}

	if(a < b&&(-7.106569030933685e-277<=a)&&(a<=-6.532353551725582e-277)) {
		r = sqrt(((((c + (b + a)) * (a - (c - b))) * (a + (c - b))) * (c + (b - a)))) / 4.0;
		return r;
	}

	if(a < b&&(2.5683516051161374e-257<=a)&&(a<=3.720652011351933e-257)) {
		r = sqrt(((((c + (b + a)) * (a - (c - b))) * (a + (c - b))) * (c + (b - a)))) / 4.0;
		return r;
	}

	if(a < b&&(4.963328843187051e-245<=a)&&(a<=5.001730032770586e-245)) {
		r = sqrt(((((c + (b + a)) * (a - (c - b))) * (a + (c - b))) * (c + (b - a)))) / 4.0;
		return r;
	}

	if(a < b&&(2.5192554908951825e-163<=a)&&(a<=2.6317839716959753e-163)) {
		r = sqrt(((((c + (b + a)) * (a - (c - b))) * (a + (c - b))) * (c + (b - a)))) / 4.0;
		return r;
	}

	if(a >= b&&(-2.3836811794075206e-244<=a)&&(a<=-1.59705048161082e-244)) {
		r = sqrt(((((c + (a + b)) * (b - (c - a))) * (b + (c - a))) * (c + (a - b)))) / 4.0;
		return r;
	}

	if(a >= b&&(-8.676363299047421e-277<=a)&&(a<=-8.676363299047421e-277)) {
		r = sqrt(((((c + (a + b)) * (b - (c - a))) * (b + (c - a))) * (c + (a - b)))) / 4.0;
		return r;
	}

	if(a >= b&&(1.2731075244774277e-257<=a)&&(a<=1.2731075244774277e-257)) {
		r = sqrt(((((c + (a + b)) * (b - (c - a))) * (b + (c - a))) * (c + (a - b)))) / 4.0;
		return r;
	}

	if(a >= b&&(2.821538577212522e-247<=a)&&(a<=2.821538577212522e-247)) {
		r = sqrt(((((c + (a + b)) * (b - (c - a))) * (b + (c - a))) * (c + (a - b)))) / 4.0;
		return r;
	}

	if(a >= b&&(7.71945767527287e-164<=a)&&(a<=7.71945767527287e-164)) {
		r = sqrt(((((c + (a + b)) * (b - (c - a))) * (b + (c - a))) * (c + (a - b)))) / 4.0;
		return r;
	}

	if(a < b) {
		r_real = iRRAM::sqrt((REAL)((((c_real + (b_real + a_real)) * (a_real - (c_real - b_real))) * (a_real + (c_real - b_real))) * (c_real + (b_real - a_real)))) / REAL(4.0);
		return r_real.as_double();
	}

	if(a >= b) {
		r_real = iRRAM::sqrt((REAL)((((c_real + (a_real + b_real)) * (b_real - (c_real - a_real))) * (b_real + (c_real - a_real))) * (c_real + (a_real - b_real)))) / REAL(4.0);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_triangleSorted_2_NumOpt(double x)
	{
		return expr_triangleSorted_2_CPP(x);
	}
}