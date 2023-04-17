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

double expr_smartRoot_2_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if(((3.5*3.5)-(3.0*x))>10.0&&(-1.498077612385249e+307<=x)&&(x<=0.030049679510853158)) {
		r = (x*2.0)/(-3.5-sqrt(((3.5*3.5)-((3.0*x)*4.0))));
		return r;
	}

	if(((3.5*3.5)-(3.0*x))<=10.0&&(1.0208333333332282<=x)&&(x<=1.0208333333332282)) {
		r = (x*2.0)/(-3.5-sqrt(((3.5*3.5)-((3.0*x)*4.0))));
		return r;
	}

	if(((3.5*3.5)-(3.0*x))>10.0) {
		r_real = (x_real*REAL(2.0))/(-REAL(3.5)-iRRAM::sqrt((REAL)((REAL(3.5)*REAL(3.5))-((REAL(3.0)*x_real)*REAL(4.0)))));
		return r_real.as_double();
	}

	if(((3.5*3.5)-(3.0*x))<=10.0) {
		r_real = (x_real*REAL(2.0))/(-REAL(3.5)-iRRAM::sqrt((REAL)((REAL(3.5)*REAL(3.5))-((REAL(3.0)*x_real)*REAL(4.0)))));
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_smartRoot_2_NumOpt(double x)
	{
		return expr_smartRoot_2_CPP(x);
	}
}