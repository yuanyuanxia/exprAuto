#include "mine.h"
double expr_polyIDX2_daisyArfa(double x1, double x2, double x3, double x4) {
    double result;
    if((6 < x1 && x1 < 6.6) && (1.85 < x2 && x2 < 4.3) && (-2 < x3 && x3 < 6.4) && (1.25 < x4 && x4 < 2.6)) {
		result =  (((((0.052100253064028955 * x1) + (-(0.8942341911807864) * x2)) + ((0.40478255170032273 * x3) + (-(0.9377469019770424) * x4))) * (((-(0.9377469019770424) * x4) + (((0.052100253064028955 * x1) + (-(0.8942341911807864) * x2)) + (0.40478255170032273 * x3))) * (((0.052100253064028955 * x1) + (-(0.8942341911807864) * x2)) + ((0.40478255170032273 * x3) + (-(0.9377469019770424) * x4))))) - (((((-(0.8507307281963669) * x1) + ((-(0.986640169163685) * x2) + (1.3809517485817782 * x3))) + (1.865345093933099 * x4)) * (((x1 * -(0.8507307281963669)) + ((-(0.986640169163685) * x2) + (1.3809517485817782 * x3))) + (x4 * 1.865345093933099))) * (((x1 * -(0.8507307281963669)) + (x4 * 1.865345093933099)) + ((1.3809517485817782 * x3) - (0.986640169163685 * x2)))));
    } else {
        result = (((0.052100253064028955 * x1) + (-0.8942341911807864 * x2) + (0.40478255170032273 * x3) + (-0.93774690197704236 * x4)) * ((0.052100253064028955 * x1) + (-0.8942341911807864 * x2) + (0.40478255170032273 * x3) + (-0.93774690197704236 * x4)) * ((0.052100253064028955 * x1) + (-0.8942341911807864 * x2) + (0.40478255170032273 * x3) + (-0.93774690197704236 * x4)) - ((-0.85073072819636686 * x1) + (-0.98664016916368502 * x2) + (1.3809517485817782 * x3) + (1.8653450939330991 * x4)) * ((-0.85073072819636686 * x1) + (-0.98664016916368502 * x2) + (1.3809517485817782 * x3) + (1.8653450939330991 * x4)) * ((-0.85073072819636686 * x1) + (-0.98664016916368502 * x2) + (1.3809517485817782 * x3) + (1.8653450939330991 * x4)))
;
    }
    return result;
}