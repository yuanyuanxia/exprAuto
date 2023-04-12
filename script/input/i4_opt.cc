
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


double irram_sintan(double x, double y)
{
	REAL x_real(x);
	REAL y_real(y);
	double r;
	REAL r_real;

	if((0.1<=x)&&(x<=10)&&(-5<=y)&&(y<=5)) {
		r_real = sqrtf(x_real+y_real*y_real);
		return r_real.as_double();
	}

	return r;
}

void compute() {
	std::fesetround(FE_DOWNWARD);
	std::string x_str,y_str;
	iRRAM::cin >> x_str >> y_str;
	double x_double = binary2double(x_str);
	double y_double = binary2double(y_str);
	double r_double = irram_sintan(x_double, y_double);
	iRRAM::cout << double2binary(r_double) << "\n";
}

