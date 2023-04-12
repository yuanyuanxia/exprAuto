
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

	if((0.01<=x)&&(x<=100)&&(0.08776050680208015<=x)&&(x<=0.09169948774645326)) {
		r = exp(x)/(exp(x) - 1);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.10275257911692376<=x)&&(x<=0.1415960752103245)) {
		r = exp(x)/(exp(x) - 1);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.14621514922976026<=x)&&(x<=0.15438859275988195)) {
		r = exp(x)/(exp(x) - 1);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.15506122639664188<=x)&&(x<=91.61769657003777)) {
		r = exp(x)/(exp(x) - 1);
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = iRRAM::exp((REAL)x_real)/(iRRAM::exp((REAL)x_real) - 1);
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

