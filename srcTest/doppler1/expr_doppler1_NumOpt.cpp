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

double expr_doppler1_CPP(double T, double u, double v)
{
	REAL T_real(T);
	REAL v_real(v);
	REAL u_real(u);
	double r;
	REAL r_real;

	if((-30<=T)&&(T<=50)&&(-100<=v)&&(v<=100)&&(20<=u)&&(u<=20000)&&(-7.338559287692092<=T)&&(T<=3.8325674523602244)) {
		r = ((-(1657.0/5.0+3.0/5.0*T))*v)/(((1657.0/5.0+3.0/5.0*T)+u)*((1657.0/5.0+3.0/5.0*T)+u));
		return r;
	}

	if((-30<=T)&&(T<=50)&&(-100<=v)&&(v<=100)&&(20<=u)&&(u<=20000)) {
		r_real = ((-(REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real))*v_real)/(((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real)*((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real));
		return r_real.as_double();
	}

	r_real = ((-(REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real))*v_real)/(((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real)*((REAL(1657.0)/REAL(5.0)+REAL(3.0)/REAL(5.0)*T_real)+u_real));
	return r_real.as_double();
}

extern "C"
{
	double expr_doppler1_NumOpt(double x0, double x1, double x2)
	{
		return expr_doppler1_CPP(x0, x1, x2);
	}
}