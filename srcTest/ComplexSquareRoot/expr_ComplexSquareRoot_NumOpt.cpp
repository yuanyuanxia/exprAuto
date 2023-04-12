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

double expr_ComplexSquareRoot_CPP(double x1, double x2)
{
	REAL x1_real(x1);
	REAL x2_real(x2);
	double r;
	REAL r_real;

	if((0<=x1)&&(x1<=1)&&(0<=x2)&&(x2<=1)&&(1.275086505940163e-308<=x1)&&(x1<=4.657495262892392e-06)) {
		r = 1.0/2.0*sqrt(2.0*(sqrt(x1*x1+x2*x2)+x1));
		return r;
	}

	if((0<=x1)&&(x1<=1)&&(0<=x2)&&(x2<=1)) {
		r_real = REAL(1.0)/REAL(2.0)*iRRAM::sqrt((REAL)REAL(2.0)*(iRRAM::sqrt((REAL)x1_real*x1_real+x2_real*x2_real)+x1_real));
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_ComplexSquareRoot_NumOpt(double x)
	{
		return expr_ComplexSquareRoot_CPP(x);
	}
}