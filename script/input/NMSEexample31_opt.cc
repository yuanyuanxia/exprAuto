
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

	if((0.01<=x)&&(x<=100)&&(0.01033204425169536<=x)&&(x<=3.5786479018381825)) {
		r = sqrt(x+1)-sqrt(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(4.194470440466919<=x)&&(x<=4.454459669337519)) {
		r = sqrt(x+1)-sqrt(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(5.086463095261279<=x)&&(x<=5.494538771451066)) {
		r = sqrt(x+1)-sqrt(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(5.599975109497048<=x)&&(x<=5.726539993141816)) {
		r = sqrt(x+1)-sqrt(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(6.588006724737285<=x)&&(x<=7.231828001818615)) {
		r = sqrt(x+1)-sqrt(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(7.685840646771626<=x)&&(x<=8.810461536106247)) {
		r = sqrt(x+1)-sqrt(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(9.773739960540086<=x)&&(x<=10.715677981948954)) {
		r = sqrt(x+1)-sqrt(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(12.276570111213195<=x)&&(x<=12.982537102167978)) {
		r = sqrt(x+1)-sqrt(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(14.68601137308409<=x)&&(x<=14.84871472009893)) {
		r = sqrt(x+1)-sqrt(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(55.10931785603222<=x)&&(x<=55.10931786223146)) {
		r = sqrt(x+1)-sqrt(x);
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = iRRAM::sqrt((REAL)x_real+1)-iRRAM::sqrt((REAL)x_real);
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

