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

double expr_NMSEproblem337_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.01<=x)&&(x<=100)&&(0.4479715718522299<=x)&&(x<=0.44842734433069176)) {
		r = exp(x)+(-2)+exp(-x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.49358419750059995<=x)&&(x<=96.8690865155957)) {
		r = exp(x)+(-2)+exp(-x);
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = iRRAM::exp((REAL)x_real)+(-2)+iRRAM::exp((REAL)-x_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEproblem337_NumOpt(double x)
	{
		return expr_NMSEproblem337_CPP(x);
	}
}