
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

void compute() {
	std::fesetround(FE_DOWNWARD);
	std::string x_str;
	iRRAM::cin >> x_str;
	double x_double = binary2double(x_str);
	double r_double = irram_sintan(x_double);
	iRRAM::cout << double2binary(r_double) << "\n";
}

