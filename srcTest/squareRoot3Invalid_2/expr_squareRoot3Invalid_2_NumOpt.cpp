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

double expr_squareRoot3Invalid_2_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if(x<0.0001&&(-5.849734155840792e+303<=x)&&(x<=4.8372664685901273e-05)) {
		r = 1.0+(0.5*x);
		return r;
	}

	if(x>=0.0001&&(487476.22196357907<=x)&&(x<=1.2943681476239454e+303)) {
		r = sqrt((1.0+x));
		return r;
	}

	if(x<0.0001) {
		r_real = REAL(1.0)+(REAL(0.5)*x_real);
		return r_real.as_double();
	}

	if(x>=0.0001) {
		r_real = iRRAM::sqrt((REAL)(REAL(1.0)+x_real));
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_squareRoot3Invalid_2_NumOpt(double x)
	{
		return expr_squareRoot3Invalid_2_CPP(x);
	}
}