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

double expr_sineorder3_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((-2<=x)&&(x<=2)&&(-0.7637160874038874<=x)&&(x<=0.1979489862799192)) {
		r = (238732414637843.0/250000000000000.0)*x - (6450306886639899.0/50000000000000000.0)*x*x*x;
		return r;
	}

	if((-2<=x)&&(x<=2)) {
		r_real = (REAL(238732414637843.0)/REAL(250000000000000.0))*x_real - (REAL(6450306886639899.0)/REAL(50000000000000000.0))*x_real*x_real*x_real;
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_sineorder3_NumOpt(double x)
	{
		return expr_sineorder3_CPP(x);
	}
}