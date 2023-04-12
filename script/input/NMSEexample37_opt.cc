
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

	if((0.01<=x)&&(x<=100)&&(0.06722497868733401<=x)&&(x<=0.06725235096169693)) {
		r = exp(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.11786006198612779<=x)&&(x<=0.13864989446981987)) {
		r = exp(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.141488869668345<=x)&&(x<=0.18389725264762594)) {
		r = exp(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.18718203291301166<=x)&&(x<=0.2017189063726586)) {
		r = exp(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.20382590581321117<=x)&&(x<=99.12853872052801)) {
		r = exp(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = iRRAM::exp((REAL)x_real)-1;
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

