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

double expr_doppler2_CPP(double T, double u, double v)
{
	REAL T_real(T);
	REAL v_real(v);
	REAL u_real(u);
	double r;
	REAL r_real;

	if((-40<=T)&&(T<=60)&&(-125<=v)&&(v<=125)&&(15<=u)&&(u<=25000)&&(-10.915844254234749<=T)&&(T<=4.702493440196151)) {
		r = ((-(1657.0/5.0+3.0/5.0*T))*v)/(((1657.0/5.0+3.0/5.0*T)+u)*((1657.0/5.0+3.0/5.0*T)+u));
		return r;
	}

	if((-40<=T)&&(T<=60)&&(-125<=v)&&(v<=125)&&(15<=u)&&(u<=25000)) {
		r_real = ((-(REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real))*v_real)/(((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real)*((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real));
		return r_real.as_double();
	}

	r_real = ((-(REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real))*v_real)/(((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real)*((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real));
	return r_real.as_double();
}

extern "C"
{
	double expr_doppler2_NumOpt(double x0, double x1, double x2)
	{
		return expr_doppler2_CPP(x0, x1, x2);
	}
}