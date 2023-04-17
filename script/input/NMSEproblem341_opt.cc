
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

	if((0.01<=x)&&(x<=100)&&(0.2989954160583664<=x)&&(x<=0.3040578669088568)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.31285789704270733<=x)&&(x<=0.3282430725719571)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.34870518609020895<=x)&&(x<=6.034636508320054)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(6.574480063196469<=x)&&(x<=49.988447914427496)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(50.545623864527215<=x)&&(x<=56.3274051305552)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(56.77482997906461<=x)&&(x<=94.04557348534465)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(94.50957064205821<=x)&&(x<=97.81728977191308)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = (1 -iRRAM::cos((REAL)x_real))/(x_real*x_real);
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

