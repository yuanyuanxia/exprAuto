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

double expr_doppler3_CPP(double T, double u, double v)
{
	REAL T_real(T);
	REAL v_real(v);
	REAL u_real(u);
	double r;
	REAL r_real;

	if((-50<=T)&&(T<=30)&&(-30<=v)&&(v<=120)&&(320<=u)&&(u<=20300)&&(-37.47075088910655<=T)&&(T<=0.011584792119040614)) {
		r = ((-(1657.0/5.0+3.0/5.0*T))*v)/(((1657.0/5.0+3.0/5.0*T)+u)*((1657.0/5.0+3.0/5.0*T)+u));
		return r;
	}

	if((-50<=T)&&(T<=30)&&(-30<=v)&&(v<=120)&&(320<=u)&&(u<=20300)) {
		r_real = ((-(REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real))*v_real)/(((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real)*((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real));
		return r_real.as_double();
	}

	r_real = ((-(REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real))*v_real)/(((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real)*((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real));
	return r_real.as_double();
}

extern "C"
{
	double expr_doppler3_NumOpt(double x0, double x1, double x2)
	{
		return expr_doppler3_CPP(x0, x1, x2);
	}
}