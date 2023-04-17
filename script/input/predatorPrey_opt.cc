
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

	if((0.1<=x)&&(x<=0.3)&&(0.10017796345245293<=x)&&(x<=0.29895279590701196)) {
		r = ((4.0 * x) * x) / (1.0 + ((x / 1.11) * (x / 1.11)));
		return r;
	}

	if((0.1<=x)&&(x<=0.3)) {
		r_real = ((REAL(4.0) * x_real) * x_real) / (REAL(1.0) + ((x_real / REAL(1.11)) * (x_real / REAL(1.11))));
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

