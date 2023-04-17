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

double expr_logexp_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((-8<=x)&&(x<=8)&&(-1.3012690852733133<=x)&&(x<=0.042406208954006805)) {
		r = log(exp(x)+1);
		return r;
	}

	if((-8<=x)&&(x<=8)) {
		r_real = iRRAM::log((REAL)iRRAM::exp((REAL)x_real)+1);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_logexp_NumOpt(double x)
	{
		return expr_logexp_CPP(x);
	}
}