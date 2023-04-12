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

double expr_NMSEexample34_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.01<=x)&&(x<=3)&&(0.24350074632789553<=x)&&(x<=0.243501436280844)) {
		r = (1-cos(x))/sin(x);
		return r;
	}

	if((0.01<=x)&&(x<=3)&&(0.24877069789885864<=x)&&(x<=0.2816733445771115)) {
		r = (1-cos(x))/sin(x);
		return r;
	}

	if((0.01<=x)&&(x<=3)&&(0.2816733569658207<=x)&&(x<=2.924004589698516)) {
		r = (1-cos(x))/sin(x);
		return r;
	}

	if((0.01<=x)&&(x<=3)) {
		r_real = (1-iRRAM::cos((REAL)x_real))/iRRAM::sin((REAL)x_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEexample34_NumOpt(double x)
	{
		return expr_NMSEexample34_CPP(x);
	}
}