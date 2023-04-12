
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

	if((0.01<=x)&&(x<=100)&&(0.010437469457944472<=x)&&(x<=0.3604339271759145)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.8871511248989055<=x)&&(x<=0.8874171326284639)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(1.1751887785480812<=x)&&(x<=1.415667007213509)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(1.458089294902233<=x)&&(x<=1.6865102059147628)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(1.6865344487760252<=x)&&(x<=1.7153206052609768)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(1.7479678878719356<=x)&&(x<=1.9087480406358917)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(1.9122839797370013<=x)&&(x<=1.9740300295334035)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(1.9931971810641311<=x)&&(x<=3.1877653850291634)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(3.1979651413073538<=x)&&(x<=4.213377740669251)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(4.237602197231224<=x)&&(x<=4.702916612374459)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(4.957614040315142<=x)&&(x<=6.474333065779055)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(6.898192685270763<=x)&&(x<=7.073384818146711)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(7.395087638773861<=x)&&(x<=7.422516993951036)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(7.818647478830652<=x)&&(x<=10.805647243398507)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(10.90936836311555<=x)&&(x<=11.621957942764213)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(59.36938744810223<=x)&&(x<=59.57610366236027)) {
		r = (x+1)*log(x+1)-x*log(x)-1;
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = (x_real+1)*iRRAM::log((REAL)x_real+1)-x_real*iRRAM::log((REAL)x_real)-1;
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

