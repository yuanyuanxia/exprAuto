
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


double irram_sintan(double r, double i)
{
	REAL r_real(r);
	REAL i_real(i);
	double y;
	REAL y_real;

	if((0<=r)&&(r<=1)&&(0<=i)&&(i<=1)&&(7.194192609125759e-245<=r)&&(r<=7.196891728795075e-245)) {
		y = 1.0/2.0*sin(r)*(exp(-i) - exp(i));
		return y;
	}

	if((0<=r)&&(r<=1)&&(0<=i)&&(i<=1)&&(7.685580509673756e-70<=r)&&(r<=7.703374426214096e-70)) {
		y = 1.0/2.0*sin(r)*(exp(-i) - exp(i));
		return y;
	}

	if((0<=r)&&(r<=1)&&(0<=i)&&(i<=1)) {
		y_real = REAL(1.0)/REAL(2.0)*iRRAM::sin((REAL)r_real)*(iRRAM::exp((REAL)-i_real) -iRRAM::exp((REAL)i_real));
		return y_real.as_double();
	}

	return y;
}

void compute() {
	std::fesetround(FE_DOWNWARD);
	std::string r_str,i_str;
	iRRAM::cin >> r_str >> i_str;
	double r_double = binary2double(r_str);
	double i_double = binary2double(i_str);
	double r_double = irram_sintan(r_double, i_double);
	iRRAM::cout << double2binary(r_double) << "\n";
}
