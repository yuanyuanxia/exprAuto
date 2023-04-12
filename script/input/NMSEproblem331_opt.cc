
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

	if((0.01<=x)&&(x<=100)&&(0.010168746512177272<=x)&&(x<=7.506099159368052)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(7.999930601070934<=x)&&(x<=10.861793248757207)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(11.073762383621652<=x)&&(x<=11.946056632198577)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(12.63633448852667<=x)&&(x<=13.108698126263626)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(13.961079927820576<=x)&&(x<=13.988194316081476)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(14.903645384885676<=x)&&(x<=15.072351500800046)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(16.627975680630563<=x)&&(x<=17.170042573570754)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(18.346556947226322<=x)&&(x<=18.839229212497735)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(18.895993618078137<=x)&&(x<=19.48144169206383)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(20.265252047716615<=x)&&(x<=21.833391961226663)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(26.911051511249386<=x)&&(x<=26.911051737877717)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(35.06295977463821<=x)&&(x<=35.06295977463821)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(36.87094614503849<=x)&&(x<=36.91473783652938)) {
		r = 1/(x+1)-1/x;
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = 1/(x_real+1)-1/x_real;
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

