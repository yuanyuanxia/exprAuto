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

double expr_exp1x_CPP(double x)
{
	REAL x_real(x);

	double r;
	REAL r_real;

	if((0.01<=x)&&(x<=0.5)&&(0.09366259775907677<=x)&&(x<=0.0941941332009637)) {
		r = (exp(x) - 1)/x;
		return r;
	}
	
	if((0.01<=x)&&(x<=0.5)&&(0.09454738790927672<=x)&&(x<=0.09491809413862791)) {
		r = (exp(x) - 1)/x;
		return r;
	}

	if((0.01<=x)&&(x<=0.5)&&(0.09525975290515465<=x)&&(x<=0.0954117393966348)) {
		r = (exp(x) - 1)/x;
		return r;
	}
	
	if((0.01<=x)&&(x<=0.5)&&(0.09644493151181782<=x)&&(x<=0.09931031525795438)) {
		r = (exp(x) - 1)/x;
		return r;
	}

	if((0.01<=x)&&(x<=0.5)&&(0.11032798079699918<=x)&&(x<=0.11662856329740823)) {
		r = (exp(x) - 1)/x;
		return r;
	}
	
	if((0.01<=x)&&(x<=0.5)&&(0.11690526661881576<=x)&&(x<=0.11815799959930169)) {
		r = (exp(x) - 1)/x;
		return r;
	}

	if((0.01<=x)&&(x<=0.5)&&(0.1202218966511988<=x)&&(x<=0.12988518668611435)) {
		r = (exp(x) - 1)/x;
		return r;
	}
	
	if((0.01<=x)&&(x<=0.5)&&(0.13039248351007793<=x)&&(x<=0.493319707285482)) {
		r = (exp(x) - 1)/x;
		return r;
	}
	
	if((0.01<=x)&&(x<=0.5)) {
		r_real = (iRRAM::exp((REAL)x_real) - 1)/x_real;
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_exp1x_NumOpt(double x)
	{
		return expr_exp1x_CPP(x);
	}
}
