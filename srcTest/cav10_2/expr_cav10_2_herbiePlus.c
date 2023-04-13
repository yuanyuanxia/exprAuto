double expr_cav10_2_herbiePlus(double x) {
	double tmp;
	if (((x * x) - x) >= 0.0) {
		tmp = x / 10.0;
	} else {
		tmp = (x * x) + 2.0;
	}
	return tmp;
}