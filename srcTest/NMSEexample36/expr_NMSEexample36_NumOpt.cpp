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

double expr_NMSEexample36_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.01<=x)&&(x<=100)&&(0.010673067477198568<=x)&&(x<=1.5958251267616936)) {
		r = 1/(sqrt(x))-1/sqrt(1+x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(1.5958251309025289<=x)&&(x<=2.0621189828509077)) {
		r = 1/(sqrt(x))-1/sqrt(1+x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(2.206460837078387<=x)&&(x<=2.2495889543715917)) {
		r = 1/(sqrt(x))-1/sqrt(1+x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(4.579100276778099<=x)&&(x<=4.686225042548934)) {
		r = 1/(sqrt(x))-1/sqrt(1+x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(5.273357509397678<=x)&&(x<=5.273359899623165)) {
		r = 1/(sqrt(x))-1/sqrt(1+x);
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = 1/(iRRAM::sqrt((REAL)x_real))-1/iRRAM::sqrt((REAL)1+x_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEexample36_NumOpt(double x)
	{
		return expr_NMSEexample36_CPP(x);
	}
}