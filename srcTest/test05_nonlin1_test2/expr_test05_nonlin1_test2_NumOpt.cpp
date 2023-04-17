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

double expr_test05_nonlin1_test2_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((1.00001<=x)&&(x<=2)&&(1.0052412782421196<=x)&&(x<=1.9962575350666676)) {
		r = 1.0/(1+x);
		return r;
	}

	if((1.00001<=x)&&(x<=2)) {
		r_real = REAL(1.0)/(1+x_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_test05_nonlin1_test2_NumOpt(double x)
	{
		return expr_test05_nonlin1_test2_CPP(x);
	}
}