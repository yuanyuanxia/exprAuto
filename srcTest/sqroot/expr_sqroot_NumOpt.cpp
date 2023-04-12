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

double expr_sqroot_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0<=x)&&(x<=1)&&(5.917017099304835e-308<=x)&&(x<=0.48856674503851)) {
		r = 1.0 + 0.5*x - 0.125*x*x + 0.0625*x*x*x - 0.0390625*x*x*x*x;
		return r;
	}

	if((0<=x)&&(x<=1)) {
		r_real = REAL(1.0) + REAL(0.5)*x_real - REAL(0.125)*x_real*x_real + REAL(0.0625)*x_real*x_real*x_real - REAL(0.0390625)*x_real*x_real*x_real*x_real;
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_sqroot_NumOpt(double x)
	{
		return expr_sqroot_CPP(x);
	}
}