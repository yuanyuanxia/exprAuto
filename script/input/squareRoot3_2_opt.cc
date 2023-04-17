
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

	if(x<1e-5&&(-4.884449476155393e+306<=x)&&(x<=1.6567059425414487e-14)) {
		r = 1.0+(0.5*x);
		return r;
	}

	if(x>=1e-5&&(0.2162655827879985<=x)&&(x<=7.556698972072338e+306)) {
		r = sqrt((1.0+x));
		return r;
	}

	if(x<1e-5) {
		r_real = REAL(1.0)+(REAL(0.5)*x_real);
		return r_real.as_double();
	}

	if(x>=1e-5) {
		r_real = iRRAM::sqrt((REAL)(REAL(1.0)+x_real));
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

