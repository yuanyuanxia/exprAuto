
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


double irram_sintan(double radius, double theta)
{
	REAL radius_real(radius);
	REAL theta_real(theta);
	double r;
	REAL r_real;

	if((1<=radius)&&(radius<=10)&&(0<=theta)&&(theta<=360)&&(1.010047581718489<=radius)&&(radius<=9.95152096691238)) {
		r = radius*cos(3.14159265359/180*theta);
		return r;
	}

	if((1<=radius)&&(radius<=10)&&(0<=theta)&&(theta<=360)) {
		r_real = radius_real*iRRAM::cos((REAL)REAL(3.14159265359)/180*theta_real);
		return r_real.as_double();
	}

	return r;
}

void compute() {
	std::fesetround(FE_DOWNWARD);
	std::string radius_str,theta_str;
	iRRAM::cin >> radius_str >> theta_str;
	double radius_double = binary2double(radius_str);
	double theta_double = binary2double(theta_str);
	double r_double = irram_sintan(radius_double, theta_double);
	iRRAM::cout << double2binary(r_double) << "\n";
}

