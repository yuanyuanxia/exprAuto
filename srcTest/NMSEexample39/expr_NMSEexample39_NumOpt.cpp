#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <cfenv>

#include "backend/points.h"
#include "backend/self_math.h"
#include "backend/gamma.h"
#include "iRRAM/lib.h"

int iRRAM::MAXiterationnum = 30;
bool iRRAM::enableReiterate = true;
bool iRRAM::alwaysenableReiterate = true;

using namespace std;
using namespace iRRAM;

double expr_NMSEexample39_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.01<=x)&&(x<=1)&&(0.3752304551929883<=x)&&(x<=0.3782631882078102)) {
		r = 1/x-1/tan(x);
		return r;
	}

	if((0.01<=x)&&(x<=1)&&(0.38197192351353915<=x)&&(x<=0.38339280567489276)) {
		r = 1/x-1/tan(x);
		return r;
	}

	if((0.01<=x)&&(x<=1)&&(0.3906040016481666<=x)&&(x<=0.3908873003167046)) {
		r = 1/x-1/tan(x);
		return r;
	}

	if((0.01<=x)&&(x<=1)&&(0.41124914167390286<=x)&&(x<=0.4272101777696732)) {
		r = 1/x-1/tan(x);
		return r;
	}

	if((0.01<=x)&&(x<=1)&&(0.458640562116811<=x)&&(x<=0.45959711531029157)) {
		r = 1/x-1/tan(x);
		return r;
	}

	if((0.01<=x)&&(x<=1)&&(0.47477447784129195<=x)&&(x<=0.5588437133375577)) {
		r = 1/x-1/tan(x);
		return r;
	}

	if((0.01<=x)&&(x<=1)&&(0.5588549067302485<=x)&&(x<=0.9927948509853173)) {
		r = 1/x-1/tan(x);
		return r;
	}

	if((0.01<=x)&&(x<=1)) {
		r_real = 1/x_real-1/iRRAM::tan((REAL)x_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEexample39_NumOpt(double x)
	{
		return expr_NMSEexample39_CPP(x);
	}
}