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

double expr_NMSEexample33_CPP(double eps, double x)
{
	REAL eps_real(eps);
	REAL x_real(x);
	double r;
	REAL r_real;

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(7.434734209367829e-295<=eps)&&(eps<=8.067147183600915e-295)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.8773066864894313e-252<=eps)&&(eps<=4.296923398836829e-252)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(7.292982820860915e-247<=eps)&&(eps<=4.2093941912301484e-246)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(7.439134699417557e-245<=eps)&&(eps<=3.451357033511687e-244)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.5629818809612405e-243<=eps)&&(eps<=1.749549226446628e-243)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(8.547944083624428e-242<=eps)&&(eps<=1.9135205006649776e-241)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.456150995024938e-236<=eps)&&(eps<=1.2530976124527013e-235)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.940192906463652e-222<=eps)&&(eps<=2.5233710184953186e-221)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.10584813090912e-215<=eps)&&(eps<=1.1177005878997507e-215)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.958213543508275e-214<=eps)&&(eps<=3.035842818004483e-214)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.283586601882369e-212<=eps)&&(eps<=4.971468737886205e-212)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(6.298269576390221e-205<=eps)&&(eps<=7.021665925957399e-202)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.9416485868453986e-200<=eps)&&(eps<=2.0633038726037926e-199)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(5.9225919784203144e-198<=eps)&&(eps<=7.71695143609805e-198)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(5.865050872406589e-192<=eps)&&(eps<=8.350952230606281e-191)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.5273432789945824e-186<=eps)&&(eps<=2.5963024681959457e-186)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.075517320567907e-184<=eps)&&(eps<=1.0881780473271177e-184)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.3982558177714916e-181<=eps)&&(eps<=8.418691105024768e-181)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.698921537507404e-179<=eps)&&(eps<=1.4561260987656107e-175)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.406006231055905e-173<=eps)&&(eps<=1.642061578747113e-173)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.2371439167223059e-167<=eps)&&(eps<=1.4836079964423097e-167)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(6.4605279497621005e-167<=eps)&&(eps<=1.0367565562744443e-166)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(8.196198439286282e-165<=eps)&&(eps<=4.2164099155356946e-159)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.767856708299514e-157<=eps)&&(eps<=5.736729788778377e-157)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.8984484362845337e-155<=eps)&&(eps<=8.750539427998615e-155)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(9.00880171098746e-153<=eps)&&(eps<=1.6801884003904512e-151)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.5021247214735642e-147<=eps)&&(eps<=2.724268953477039e-147)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.4779921920129862e-145<=eps)&&(eps<=3.2954631442243097e-143)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.3537520997688223e-136<=eps)&&(eps<=5.4971274033141563e-135)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(5.245392262412381e-132<=eps)&&(eps<=1.7793896213428044e-130)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.4440435724332697e-126<=eps)&&(eps<=5.546018879285196e-122)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.201105631704275e-119<=eps)&&(eps<=1.050870047393311e-118)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(5.570725412228856e-116<=eps)&&(eps<=1.9443199258523357e-114)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.1171459485515486e-112<=eps)&&(eps<=1.1637929873374265e-111)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(3.108708060002273e-110<=eps)&&(eps<=5.209767174467671e-110)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(5.39402231747422e-109<=eps)&&(eps<=8.826602772123591e-103)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.3802083625741864e-101<=eps)&&(eps<=9.418828959099321e-101)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.2155818321423112e-98<=eps)&&(eps<=2.807433808214297e-97)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.5061298504327682e-96<=eps)&&(eps<=1.2818341631171205e-95)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(4.211203409411544e-94<=eps)&&(eps<=5.723623564850643e-83)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.662671903138479e-80<=eps)&&(eps<=7.12104770074523e-54)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(7.045892075217811e-53<=eps)&&(eps<=1.6416534859934596e-50)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(8.493703732142176e-49<=eps)&&(eps<=1.3664606924288116e-42)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(1.1892890898562904e-41<=eps)&&(eps<=3.088334301914156e-38)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(2.1780907232134085e-36<=eps)&&(eps<=6.212899569162941e-20)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)&&(5.7025994319818814e-15<=eps)&&(eps<=0.08788607496239624)) {
		r = sin(x+eps) - sin(x);
		return r;
	}

	if((0<=eps)&&(eps<=100)&&(0<=x)&&(x<=100)) {
		r_real = iRRAM::sin((REAL)x_real+eps_real) -iRRAM::sin((REAL)x_real);
		return r_real.as_double();
	}

	return r;
}

extern "C"
{
	double expr_NMSEexample33_NumOpt(double x)
	{
		return expr_NMSEexample33_CPP(x);
	}
}