
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

	if((1.00001<=x)&&(x<=2)&&(1.0552419072003931<=x)&&(x<=1.0552419120606928)) {
		r = (x - 1)/(x*x - 1);
		return r;
	}

	if((1.00001<=x)&&(x<=2)&&(1.0741413908227626<=x)&&(x<=1.0771392064647325)) {
		r = (x - 1)/(x*x - 1);
		return r;
	}

	if((1.00001<=x)&&(x<=2)&&(1.0867648158267522<=x)&&(x<=1.9975838929838903)) {
		r = (x - 1)/(x*x - 1);
		return r;
	}

	if((1.00001<=x)&&(x<=2)) {
		r_real = (x_real - 1)/(x_real*x_real - 1);
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

