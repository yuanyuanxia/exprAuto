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

double expr_intro_example_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0<=x)&&(x<=999)&&(4.006327493609276e-308<=x)&&(x<=627.2207225851557)) {
		r = x/(x+1);
		return r;
	}

	if((0<=x)&&(x<=999)) {
		r_real = x_real/(x_real+1);
		return r_real.as_double();
	}
	r_real = x_real/(x_real+1);
	return r_real.as_double();
}

extern "C"
{
	double expr_intro_example_NumOpt(double x)
	{
		return expr_intro_example_CPP(x);
	}
}