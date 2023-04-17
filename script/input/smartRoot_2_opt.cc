
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

	if(((3.5*3.5)-(3.0*x))>10.0&&(-1.498077612385249e+307<=x)&&(x<=0.030049679510853158)) {
		r = (x*2.0)/(-3.5-sqrt(((3.5*3.5)-((3.0*x)*4.0))));
		return r;
	}

	if(((3.5*3.5)-(3.0*x))<=10.0&&(1.0208333333332282<=x)&&(x<=1.0208333333332282)) {
		r = (x*2.0)/(-3.5-sqrt(((3.5*3.5)-((3.0*x)*4.0))));
		return r;
	}

	if(((3.5*3.5)-(3.0*x))>10.0) {
		r_real = (x_real*REAL(2.0))/(-REAL(3.5)-iRRAM::sqrt((REAL)((REAL(3.5)*REAL(3.5))-((REAL(3.0)*x_real)*REAL(4.0)))));
		return r_real.as_double();
	}

	if(((3.5*3.5)-(3.0*x))<=10.0) {
		r_real = (x_real*REAL(2.0))/(-REAL(3.5)-iRRAM::sqrt((REAL)((REAL(3.5)*REAL(3.5))-((REAL(3.0)*x_real)*REAL(4.0)))));
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

