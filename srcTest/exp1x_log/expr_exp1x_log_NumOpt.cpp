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

double expr_exp1x_log_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.01<=x)&&(x<=0.5)&&(0.010023301815109261<=x)&&(x<=0.4988482364557297)) {
		r = (exp(x) - 1)/log(exp(x));
		return r;
	}

	if((0.01<=x)&&(x<=0.5)) {
		r_real = (iRRAM::exp((REAL)x_real) - 1)/iRRAM::log((REAL)iRRAM::exp((REAL)x_real));
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_exp1x_log_NumOpt(double x)
	{
		return expr_exp1x_log_CPP(x);
	}
}