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

double expr_NMSEproblem344_CPP(double x)
{
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0.01<=x)&&(x<=100)&&(0.013357758595457445<=x)&&(x<=0.013385753711132663)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.02645968602728466<=x)&&(x<=0.02645980091091372)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.0308420310518845<=x)&&(x<=0.030958041677699714)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.03450520733174101<=x)&&(x<=0.03493048023928692)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.03648830890925827<=x)&&(x<=0.037239332545066846)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.03788666905979504<=x)&&(x<=0.03880369998557144)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.042739541918337405<=x)&&(x<=0.045849343864546734)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.047851128580444706<=x)&&(x<=0.04888244798170545)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.04895292466058827<=x)&&(x<=0.053780429681729275)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.054387927814817334<=x)&&(x<=0.05630020054267815)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.05630374551650564<=x)&&(x<=0.05808592715342758)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.058318470703043156<=x)&&(x<=0.06031276239723299)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)&&(0.06045652094587757<=x)&&(x<=98.60588550090255)) {
		r = sqrt((exp(2*x) - 1)/(exp(x) - 1));
		return r;
	}

	if((0.01<=x)&&(x<=100)) {
		r_real = iRRAM::sqrt((REAL)(iRRAM::exp((REAL)2*x_real) - 1)/(iRRAM::exp((REAL)x_real) - 1));
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEproblem344_NumOpt(double x)
	{
		return expr_NMSEproblem344_CPP(x);
	}
}