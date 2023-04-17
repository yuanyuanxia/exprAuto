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

double expr_squareRoot3_2_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if(x<1e-5&&(-4.884449476155393e+306<=x)&&(x<=1.6567059425414487e-14)) {
		r = 1.0+(0.5*x);
		return r;
	}

	if(x>=1e-5&&(0.2162655827879985<=x)&&(x<=7.556698972072338e+306)) {
		r = sqrt((1.0+x));
		return r;
	}

	if(x<1e-5) {
		r_real = REAL(1.0)+(REAL(0.5)*x_real);
		return r_real.as_double();
	}

	if(x>=1e-5) {
		r_real = iRRAM::sqrt((REAL)(REAL(1.0)+x_real));
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_squareRoot3_2_NumOpt(double x)
	{
		return expr_squareRoot3_2_CPP(x);
	}
}