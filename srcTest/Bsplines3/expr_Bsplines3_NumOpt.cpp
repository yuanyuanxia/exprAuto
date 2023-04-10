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

double expr_Bsplines3_CPP(double x)
{
	REAL x_real(x);

	REAL pp(6);
	double r;
	REAL r_real;

	if((0<=x)&&(x<=1)&&(7.046648899479229e-308<=x)&&(x<=0.012096578909536646)) {
		r = -x*x*x/6;
		return r;
	}

	if((0<=x)&&(x<=1)) {
		r_real = -x_real*x_real*x_real/6;
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_Bsplines3_NumOpt(double x)
	{
		return expr_Bsplines3_CPP(x);
	}
}
