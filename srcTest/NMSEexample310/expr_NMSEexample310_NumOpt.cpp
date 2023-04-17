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

double expr_NMSEexample310_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((-1<=x)&&(x<=1)) {
		r_real = iRRAM::log((REAL)1-x_real)/iRRAM::log((REAL)1+x_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEexample310_NumOpt(double x)
	{
		return expr_NMSEexample310_CPP(x);
	}
}