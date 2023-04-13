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

double expr_pov_ray_CPP(double x1, double x2, double x3, double x4, double x5)
{
	REAL x1_real(x1);
	REAL x2_real(x2);
	REAL x3_real(x3);
	REAL x4_real(x4);
	REAL x5_real(x5);
	double r;
	REAL r_real;

	if(((-1 <= x1) && (x1 <= 1) && (-1<=x5) && (x5<=1) && (-3.1415926535897932384626433832795 <= x2) && (x2 <= 3.1415926535897932384626433832795) && (-3.1415926535897932384626433832795 <= x3) && (x3 <= 3.1415926535897932384626433832795) && (-1 <= x4) && (x4 <= 1))&&(-4.9361672509868313e-05<=x1)&&(x1<=0.00012576726982661083)) {
		r = x1*cos(x2)*cos(x3)+x4*cos(x2)+x5*cos(x2)*sin(x3);
		return r;
	}

	if(((-1 <= x1) && (x1 <= 1) && (-1<=x5) && (x5<=1) && (-3.1415926535897932384626433832795 <= x2) && (x2 <= 3.1415926535897932384626433832795) && (-3.1415926535897932384626433832795 <= x3) && (x3 <= 3.1415926535897932384626433832795) && (-1 <= x4) && (x4 <= 1))) {
		r_real = x1_real*iRRAM::cos((REAL)x2_real)*iRRAM::cos((REAL)x3_real)+x4_real*iRRAM::cos((REAL)x2_real)+x5_real*iRRAM::cos((REAL)x2_real)*iRRAM::sin((REAL)x3_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_pov_ray_NumOpt(double x)
	{
		return expr_pov_ray_CPP(x);
	}
}