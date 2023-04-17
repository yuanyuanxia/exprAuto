
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

	if((x*x)-x>=0&&(-6.083282645501321e+303<=x)&&(x<=3.875119102832546e+307)) {
		r = x/10;
		return r;
	}

	if((x*x)-x<0&&(1.114287922540996e-298<=x)&&(x<=1.2770265159116257e-05)) {
		r = x*x+2;
		return r;
	}

	if((x*x)-x>=0) {
		r_real = x_real/10;
		return r_real.as_double();
	}

	if((x*x)-x<0) {
		r_real = x_real*x_real+2;
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

