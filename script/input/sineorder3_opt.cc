
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

	if((-2<=x)&&(x<=2)&&(-0.7637160874038874<=x)&&(x<=0.1979489862799192)) {
		r = (238732414637843.0/250000000000000.0)*x - (6450306886639899.0/50000000000000000.0)*x*x*x;
		return r;
	}

	if((-2<=x)&&(x<=2)) {
		r_real = (REAL(238732414637843.0)/REAL(250000000000000.0))*x_real - (REAL(6450306886639899.0)/REAL(50000000000000000.0))*x_real*x_real*x_real;
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

