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

double expr_NMSEexample35_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.01<=x)&&(x<=100)&&(0.010626357637108582<=x)&&(x<=2.4216457747634084)) {
		r = atan(x+1)-atan(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(2.4675257629085374<=x)&&(x<=2.929090134073094)) {
		r = atan(x+1)-atan(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(2.96296978663543<=x)&&(x<=3.362233496533575)) {
		r = atan(x+1)-atan(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = iRRAM::atan((REAL)x_real+1)-iRRAM::atan((REAL)x_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEexample35_NumOpt(double x)
	{
		return expr_NMSEexample35_CPP(x);
	}
}