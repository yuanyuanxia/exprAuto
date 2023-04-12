
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

	if((0.01<=x)&&(x<=3)&&(0.24350074632789553<=x)&&(x<=0.243501436280844)) {
		r = (1-cos(x))/sin(x);
		return r;
	}

	if((0.01<=x)&&(x<=3)&&(0.24877069789885864<=x)&&(x<=0.2816733445771115)) {
		r = (1-cos(x))/sin(x);
		return r;
	}

	if((0.01<=x)&&(x<=3)&&(0.2816733569658207<=x)&&(x<=2.924004589698516)) {
		r = (1-cos(x))/sin(x);
		return r;
	}

	if((0.01<=x)&&(x<=3)) {
		r_real = (1-iRRAM::cos((REAL)x_real))/iRRAM::sin((REAL)x_real);
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

