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

double expr_NMSEproblem341_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.01<=x)&&(x<=100)&&(0.2989954160583664<=x)&&(x<=0.3040578669088568)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.31285789704270733<=x)&&(x<=0.3282430725719571)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.34870518609020895<=x)&&(x<=6.034636508320054)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(6.574480063196469<=x)&&(x<=49.988447914427496)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(50.545623864527215<=x)&&(x<=56.3274051305552)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(56.77482997906461<=x)&&(x<=94.04557348534465)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(94.50957064205821<=x)&&(x<=97.81728977191308)) {
		r = (1 - cos(x))/(x*x);
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = (1 -iRRAM::cos((REAL)x_real))/(x_real*x_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEproblem341_NumOpt(double x)
	{
		return expr_NMSEproblem341_CPP(x);
	}
}