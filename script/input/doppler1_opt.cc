
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


double irram_sintan(double T, double v, double u)
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

	return r;
}

void compute() {
	std::fesetround(FE_DOWNWARD);
	std::string T_str,v_str,u_str;
	iRRAM::cin >> T_str >> v_str >> u_str;
	double T_double = binary2double(T_str);
	double v_double = binary2double(v_str);
	double u_double = binary2double(u_str);
	double r_double = irram_sintan(T_double, v_double, u_double);
	iRRAM::cout << double2binary(r_double) << "\n";
}

