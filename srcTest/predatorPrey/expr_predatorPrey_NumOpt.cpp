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

double expr_predatorPrey_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.1<=x)&&(x<=0.3)&&(0.10017796345245293<=x)&&(x<=0.29895279590701196)) {
		r = ((4.0 * x) * x) / (1.0 + ((x / 1.11) * (x / 1.11)));
		return r;
	}

	if((0.1<=x)&&(x<=0.3)) {
		r_real = ((REAL(4.0) * x_real) * x_real) / (REAL(1.0) + ((x_real / REAL(1.11)) * (x_real / REAL(1.11))));
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_predatorPrey_NumOpt(double x)
	{
		return expr_predatorPrey_CPP(x);
	}
}