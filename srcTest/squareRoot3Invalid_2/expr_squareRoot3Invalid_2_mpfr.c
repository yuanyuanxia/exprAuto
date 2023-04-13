#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include <mpfr.h>

int expr_squareRoot3Invalid_1_mpfr(double x, mpfr_t mpfrResult) {
	int status = 0;
	mpfr_t mp1, mp2, mp3, mp4, mp5;
	mpfr_init2(mp1, 128);
	mpfr_init2(mp2, 128);
	mpfr_init2(mp3, 128);
	mpfr_init2(mp4, 128);
	mpfr_init2(mp5, 128);
	mpfr_set_d(mp1, 1, MPFR_RNDN);
	mpfr_set_d(mp2, 0.5, MPFR_RNDN);
	mpfr_set_d(mp3, x, MPFR_RNDN);
	mpfr_mul(mp4, mp2, mp3, MPFR_RNDN);
	mpfr_add(mp5, mp1, mp4, MPFR_RNDN);

	mpfr_set(mpfrResult, mp5, MPFR_RNDN);
	mpfr_clear(mp1);
	mpfr_clear(mp2);
	mpfr_clear(mp3);
	mpfr_clear(mp4);
	mpfr_clear(mp5);
	return status;
}

int expr_squareRoot3Invalid_2_mpfr_old(double x, mpfr_t mpfrResult) {
	int status = 0;
	mpfr_t mp1, mp2, mp3, mp4;
	mpfr_init2(mp1, 128);
	mpfr_init2(mp2, 128);
	mpfr_init2(mp3, 128);
	mpfr_init2(mp4, 128);
	mpfr_set_d(mp1, 1, MPFR_RNDN);
	mpfr_set_d(mp2, x, MPFR_RNDN);
	mpfr_add(mp3, mp1, mp2, MPFR_RNDN);
	mpfr_sqrt(mp4, mp3, MPFR_RNDN);

	mpfr_set(mpfrResult, mp4, MPFR_RNDN);
	mpfr_clear(mp1);
	mpfr_clear(mp2);
	mpfr_clear(mp3);
	mpfr_clear(mp4);
	return status;
}

int expr_squareRoot3Invalid_2_mpfr(double x, mpfr_t mpfrResult) {
	if (x < 0.0001) {
		expr_squareRoot3Invalid_1_mpfr(x, mpfrResult);
	} else {
		expr_squareRoot3Invalid_2_mpfr_old(x, mpfrResult);
	}
}