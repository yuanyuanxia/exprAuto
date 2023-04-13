#include <math.h>
double expr_smartRoot_2_herbieArfa(double c) {
	if (c < 0.75) {
        return 2.0 * (c / (-3.5 - sqrt((12.25 + (c * -12.0)))));
	} else {
        return (-3.5 + sqrt(fma(c, -12.0, 12.25))) / 6.0;
	}
}
