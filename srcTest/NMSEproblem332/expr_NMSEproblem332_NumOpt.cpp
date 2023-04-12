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

double expr_NMSEproblem332_CPP(double eps, double x)
{
	REAL eps_real(eps);
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.5108525710296906e-259<=eps)&&(eps<=1.3810372471893627e-258)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.2837283298644606e-257<=eps)&&(eps<=1.485693192901714e-257)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.5937379220003817e-250<=eps)&&(eps<=2.6338577988520412e-250)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.5873253771292057e-243<=eps)&&(eps<=4.3137968058495976e-243)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.258410397322062e-240<=eps)&&(eps<=1.070461965396641e-239)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.8116174241174647e-236<=eps)&&(eps<=4.502548001200871e-235)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.8668774180497522e-233<=eps)&&(eps<=4.059126848953666e-233)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.0380679275377245e-232<=eps)&&(eps<=1.8130696239647908e-230)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.4115886009494417e-228<=eps)&&(eps<=7.348039670501258e-228)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.296366325211081e-214<=eps)&&(eps<=5.202244574733544e-214)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.2990911370343056e-211<=eps)&&(eps<=2.900358433758034e-211)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(6.796385350766496e-201<=eps)&&(eps<=1.058184120660101e-200)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(5.872957808587388e-199<=eps)&&(eps<=7.722773529155683e-198)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.396128270401295e-197<=eps)&&(eps<=6.934932706648367e-197)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.3452190080741407e-194<=eps)&&(eps<=6.522728310635985e-193)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.3423510789598074e-185<=eps)&&(eps<=2.3979572245317566e-185)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.1608154753405134e-185<=eps)&&(eps<=4.203138335661809e-185)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.942787053495008e-180<=eps)&&(eps<=4.339783455751588e-180)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.8426023458269196e-176<=eps)&&(eps<=3.4305606070893033e-175)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.856378759585273e-173<=eps)&&(eps<=8.383318704711661e-173)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.3358948833316437e-164<=eps)&&(eps<=5.583945808137087e-163)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(7.658939752271011e-161<=eps)&&(eps<=1.2215141607506556e-160)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.141841651104984e-160<=eps)&&(eps<=1.0458026362338476e-159)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.769600169155007e-157<=eps)&&(eps<=3.9010899624253364e-150)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.990755229480191e-143<=eps)&&(eps<=2.017765760240066e-140)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.4616015127320596e-137<=eps)&&(eps<=1.2445066320222767e-136)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(8.735155558193516e-136<=eps)&&(eps<=1.3124173303111485e-135)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.6159948666980727e-134<=eps)&&(eps<=7.502428578473285e-134)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.5037755696477674e-131<=eps)&&(eps<=3.556800900319338e-130)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(8.124511117424646e-125<=eps)&&(eps<=4.591928032598656e-121)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(8.001268481853495e-121<=eps)&&(eps<=2.5800394856144636e-120)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(7.545526506227277e-119<=eps)&&(eps<=7.71784359835738e-112)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(9.72296697448404e-109<=eps)&&(eps<=5.511761180031092e-108)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.569352435377055e-104<=eps)&&(eps<=3.955947809938828e-96)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.5811022041334388e-95<=eps)&&(eps<=1.0290556941081322e-92)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.894108265478157e-92<=eps)&&(eps<=1.7541545378895835e-91)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(6.346060764557866e-91<=eps)&&(eps<=1.8143292138843098e-90)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(6.30398962710292e-90<=eps)&&(eps<=5.3790146277372494e-89)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(5.788085951764226e-88<=eps)&&(eps<=3.240135597120385e-85)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.1899051677613604e-84<=eps)&&(eps<=3.1780907922141284e-84)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.5565651946838346e-83<=eps)&&(eps<=3.0079477103113185e-80)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(9.347152145490561e-78<=eps)&&(eps<=1.5274832674456632e-76)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.0869520731939839e-75<=eps)&&(eps<=8.425083932230819e-75)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(6.394161166053848e-74<=eps)&&(eps<=2.3187369376982203e-73)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.615442602928697e-71<=eps)&&(eps<=4.609688418154432e-60)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.2695680292935905e-59<=eps)&&(eps<=9.682701544335003e-55)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(6.541818920111603e-54<=eps)&&(eps<=1.8725260274596118e-53)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.9384543331209372e-50<=eps)&&(eps<=2.1759563993712245e-24)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.0439747553056098e-23<=eps)&&(eps<=0.00022010511756444742)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(0.00082707746975666<=eps)&&(eps<=1.346717182916112)) {
		r = tan(x+eps) - tan(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)) {
		r_real = iRRAM::tan((REAL)x_real+eps_real) -iRRAM::tan((REAL)x_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEproblem332_NumOpt(double x)
	{
		return expr_NMSEproblem332_CPP(x);
	}
}