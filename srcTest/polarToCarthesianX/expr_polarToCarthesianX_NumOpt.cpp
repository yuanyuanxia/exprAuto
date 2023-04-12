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

double expr_polarToCarthesianX_CPP(double radius, double theta)
{
	REAL radius_real(radius);
	REAL theta_real(theta);
	double r;
	REAL r_real;

	if((1<=radius)&&(radius<=10)&&(0<=theta)&&(theta<=360)&&(1.010047581718489<=radius)&&(radius<=9.95152096691238)) {
		r = radius*cos(3.14159265359/180*theta);
		return r;
	}

	if((1<=radius)&&(radius<=10)&&(0<=theta)&&(theta<=360)) {
		r_real = radius_real*iRRAM::cos((REAL)REAL(3.14159265359)/180*theta_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_polarToCarthesianX_NumOpt(double x)
	{
		return expr_polarToCarthesianX_CPP(x);
	}
}