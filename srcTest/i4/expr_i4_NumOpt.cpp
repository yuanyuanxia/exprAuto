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

double expr_i4_CPP(double x, double y)
{
	REAL x_real(x);
	REAL y_real(y);
	double r;
	REAL r_real;

	if((0.1<=x)&&(x<=10)&&(-5<=y)&&(y<=5)) {
		r_real = sqrtf(x_real+y_real*y_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_i4_NumOpt(double x0, double x1)
	{
		return expr_i4_CPP(x0, x1);
	}
}