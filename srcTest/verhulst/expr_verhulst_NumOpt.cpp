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

double expr_verhulst_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.1<=x)&&(x<=0.3)&&(0.10016810202123817<=x)&&(x<=0.29944364568656406)) {
		r = (4*x)/(1+x/1.11);
		return r;
	}

	if((0.1<=x)&&(x<=0.3)) {
		r_real = (4*x_real)/(1+x_real/REAL(1.11));
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_verhulst_NumOpt(double x)
	{
		return expr_verhulst_CPP(x);
	}
}