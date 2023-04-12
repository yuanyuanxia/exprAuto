
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

	if((0.01<=x)&&(x<=100)&&(0.4479715718522299<=x)&&(x<=0.44842734433069176)) {
		r = exp(x)+(-2)+exp(-x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.49358419750059995<=x)&&(x<=96.8690865155957)) {
		r = exp(x)+(-2)+exp(-x);
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = iRRAM::exp((REAL)x_real)+(-2)+iRRAM::exp((REAL)-x_real);
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

