
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

	if((0.01<=x)&&(x<=100)&&(0.010135723879275712<=x)&&(x<=3.6176135153984816)) {
		r = log(x+1)-log(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(3.6176135157657585<=x)&&(x<=5.136334826947236)) {
		r = log(x+1)-log(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(5.1363348269486195<=x)&&(x<=6.399636245177773)) {
		r = log(x+1)-log(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(6.479433154387088<=x)&&(x<=6.8201106928635555)) {
		r = log(x+1)-log(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(7.174341807867794<=x)&&(x<=7.509392298899693)) {
		r = log(x+1)-log(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(12.572681689919902<=x)&&(x<=12.576213687473984)) {
		r = log(x+1)-log(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = iRRAM::log((REAL)x_real+1)-iRRAM::log((REAL)x_real);
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

