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

double expr_NMSEproblem333_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.01<=x)&&(x<=0.99)&&(0.010391799439447024<=x)&&(x<=0.9778015263847194)) {
		r = 1/(x+1)-2/x + 1/(x-1);
		return r;
	}

	if((0.01<=x)&&(x<=0.99)) {
		r_real = 1/(x_real+1)-2/x_real + 1/(x_real-1);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEproblem333_NumOpt(double x)
	{
		return expr_NMSEproblem333_CPP(x);
	}
}