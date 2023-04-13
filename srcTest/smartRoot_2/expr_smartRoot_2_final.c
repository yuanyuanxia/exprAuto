#include "mine.h"
double expr_smartRoot_2_final(double c) {
    double result;
	if (c < 0.75) {
        if((-1.05741 < c && c < -1.02509)) {
            result = ((2.0 * c) / (-3.5 + (-1.0 * sqrt(((-12.0 * c) + 12.25)))));
        } else {
            result = (c * 2.0) / (-3.5 - sqrt(((3.5 * 3.5) - ((3.0 * c) * 4.0))));
        }
        return result;
	} else {
        if((0.769698 < c && c < 1.02083)) {
            result = ((-3.5 + sqrt(fma(-12.0, c, 12.25))) / 6.0);
        } else {
            result = (-3.5 + sqrt(((3.5 * 3.5) - ((3.0 * c) * 4.0)))) / (3.0 * 2.0);
        }
        return result;
	}
	return result;
}
