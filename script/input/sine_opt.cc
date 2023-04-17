
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <cfenv>

#include "iRRAM.h"
#include "../../../../Source/backend/points.h"
#include "../../../../Source/backend/self_math.h"
#include "../../../../Source/backend/gamma.h"
#include "iRRAM/lib.h"

int iRRAM::MAXiterationnum = 30;
bool iRRAM::enableReiterate  = true;
bool iRRAM::alwaysenableReiterate  = true;

using namespace std;
using namespace iRRAM;


double irram_sintan(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((-1.57079632679<=x)&&(x<=1.57079632679)&&(-0.8284989478914876<=x)&&(x<=0.00033896859562810124)) {
		r = x - (1.0/6.0)*x*x*x+(1.0/120.0)*x*x*x*x*x - (1.0/5040.0)*x*x*x*x*x*x*x;
		return r;
	}

	if((-1.57079632679<=x)&&(x<=1.57079632679)) {
		r_real = x_real - (REAL(1.0)/REAL(6.0))*x_real*x_real*x_real+(REAL(1.0)/REAL(120.0))*x_real*x_real*x_real*x_real*x_real - (REAL(1.0)/REAL(5040.0))*x_real*x_real*x_real*x_real*x_real*x_real*x_real;
		return r_real.as_double();
	}

	return r;
}

void compute() {
	std::fesetround(FE_DOWNWARD);
	std::string x_str;
	iRRAM::cin >> x_str;
	double x_double = binary2double(x_str);
	double r_double = irram_sintan(x_double);
	iRRAM::cout << double2binary(r_double) << "\n";
}

