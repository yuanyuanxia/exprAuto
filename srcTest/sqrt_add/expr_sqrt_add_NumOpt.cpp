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

double expr_sqrt_add_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((1<=x)&&(x<=1000)&&(1.0360717432848836<=x)&&(x<=974.7054355731744)) {
		r = 1/(sqrt(x + 1) + sqrt(x));
		return r;
	}

	if((1<=x)&&(x<=1000)) {
		r_real = 1/(iRRAM::sqrt((REAL)x_real + 1) +iRRAM::sqrt((REAL)x_real));
		return r_real.as_double();
	}

	r_real = 1/(iRRAM::sqrt((REAL)x_real + 1) +iRRAM::sqrt((REAL)x_real));
	return r_real.as_double();
}

extern "C"
{
	double expr_sqrt_add_NumOpt(double x)
	{
		return expr_sqrt_add_CPP(x);
	}
}