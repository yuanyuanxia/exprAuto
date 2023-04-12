
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


double irram_sintan(double eps, double x)
{
	REAL eps_real(eps);
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(6.836365780936997e-307<=eps)&&(eps<=3.424964247987962e-286)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.4726572749881702e-284<=eps)&&(eps<=4.899721680844785e-276)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(9.80094852128588e-274<=eps)&&(eps<=1.3220756303131204e-273)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.341152635398035e-273<=eps)&&(eps<=2.918900847028547e-271)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(9.29909937817375e-271<=eps)&&(eps<=2.897571946081135e-268)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.621387621444098e-268<=eps)&&(eps<=1.1810455403243362e-262)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.514498316088665e-261<=eps)&&(eps<=3.098643090316515e-237)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.1653601092697618e-236<=eps)&&(eps<=1.3277313550692224e-234)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.4228950186893905e-232<=eps)&&(eps<=3.6394307297982443e-231)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.6441878125343605e-230<=eps)&&(eps<=3.481499204600951e-230)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.9298247866138715e-228<=eps)&&(eps<=9.663047728349257e-228)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.857761876152504e-225<=eps)&&(eps<=4.879029707768337e-225)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(5.071205544053575e-225<=eps)&&(eps<=7.091595230411618e-215)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.5762797129048622e-213<=eps)&&(eps<=3.5282659320622263e-211)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.0591935467739667e-207<=eps)&&(eps<=2.8599597521194703e-200)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.9584125502794065e-199<=eps)&&(eps<=2.231979479424809e-198)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.193289562872563e-194<=eps)&&(eps<=1.1553953239314083e-188)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.0905545118199726e-186<=eps)&&(eps<=6.688161258098464e-185)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.441837659122788e-182<=eps)&&(eps<=2.461687324698518e-182)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.8064388963675838e-177<=eps)&&(eps<=7.585816439837246e-176)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.585873421513115e-171<=eps)&&(eps<=7.329270630732846e-168)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.275566960818706<=eps)&&(eps<=52.008103578053365)) {
		r = cos(x+eps) - cos(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)) {
		r_real = iRRAM::cos((REAL)x_real+eps_real) -iRRAM::cos((REAL)x_real);
		return r_real.as_double();
	}

	return r;
}

void compute() {
	std::fesetround(FE_DOWNWARD);
	std::string eps_str,x_str;
	iRRAM::cin >> eps_str >> x_str;
	double eps_double = binary2double(eps_str);
	double x_double = binary2double(x_str);
	double r_double = irram_sintan(eps_double, x_double);
	iRRAM::cout << double2binary(r_double) << "\n";
}

