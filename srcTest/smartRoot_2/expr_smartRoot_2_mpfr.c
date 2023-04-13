#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include <mpfr.h>

int expr_smartRoot_1_mpfr(double c, mpfr_t mpfrResult) {
	int status = 0;
	mpfr_t mp1, mp2, mp3, mp4, mp5, mp6, mp7, mp8, mp9, mp10, mp11, mp12, mp13, mp14, mp15, mp16, mp17, mp18;
	mpfr_init2(mp1, 128);
	mpfr_init2(mp2, 128);
	mpfr_init2(mp3, 128);
	mpfr_init2(mp4, 128);
	mpfr_init2(mp5, 128);
	mpfr_init2(mp6, 128);
	mpfr_init2(mp7, 128);
	mpfr_init2(mp8, 128);
	mpfr_init2(mp9, 128);
	mpfr_init2(mp10, 128);
	mpfr_init2(mp11, 128);
	mpfr_init2(mp12, 128);
	mpfr_init2(mp13, 128);
	mpfr_init2(mp14, 128);
	mpfr_init2(mp15, 128);
	mpfr_init2(mp16, 128);
	mpfr_init2(mp17, 128);
	mpfr_init2(mp18, 128);
	mpfr_set_d(mp1, c, MPFR_RNDN);
	mpfr_set_d(mp2, 2, MPFR_RNDN);
	mpfr_mul(mp3, mp1, mp2, MPFR_RNDN);
	mpfr_set_d(mp4, -1, MPFR_RNDN);
	mpfr_set_d(mp5, 3.5, MPFR_RNDN);
	mpfr_mul(mp6, mp4, mp5, MPFR_RNDN);
	mpfr_set_d(mp7, 3.5, MPFR_RNDN);
	mpfr_set_d(mp8, 3.5, MPFR_RNDN);
	mpfr_mul(mp9, mp7, mp8, MPFR_RNDN);
	mpfr_set_d(mp10, 3, MPFR_RNDN);
	mpfr_set_d(mp11, c, MPFR_RNDN);
	mpfr_mul(mp12, mp10, mp11, MPFR_RNDN);
	mpfr_set_d(mp13, 4, MPFR_RNDN);
	mpfr_mul(mp14, mp12, mp13, MPFR_RNDN);
	mpfr_sub(mp15, mp9, mp14, MPFR_RNDN);
	mpfr_sqrt(mp16, mp15, MPFR_RNDN);
	mpfr_sub(mp17, mp6, mp16, MPFR_RNDN);
	mpfr_div(mp18, mp3, mp17, MPFR_RNDN);

	mpfr_set(mpfrResult, mp18, MPFR_RNDN);
	mpfr_clear(mp1);
	mpfr_clear(mp2);
	mpfr_clear(mp3);
	mpfr_clear(mp4);
	mpfr_clear(mp5);
	mpfr_clear(mp6);
	mpfr_clear(mp7);
	mpfr_clear(mp8);
	mpfr_clear(mp9);
	mpfr_clear(mp10);
	mpfr_clear(mp11);
	mpfr_clear(mp12);
	mpfr_clear(mp13);
	mpfr_clear(mp14);
	mpfr_clear(mp15);
	mpfr_clear(mp16);
	mpfr_clear(mp17);
	mpfr_clear(mp18);
	return status;
}

int expr_smartRoot_2_mpfr_old(double c, mpfr_t mpfrResult) {
	int status = 0;
	mpfr_t mp1, mp2, mp3, mp4, mp5, mp6, mp7, mp8, mp9, mp10, mp11, mp12, mp13, mp14, mp15, mp16, mp17, mp18;
	mpfr_init2(mp1, 128);
	mpfr_init2(mp2, 128);
	mpfr_init2(mp3, 128);
	mpfr_init2(mp4, 128);
	mpfr_init2(mp5, 128);
	mpfr_init2(mp6, 128);
	mpfr_init2(mp7, 128);
	mpfr_init2(mp8, 128);
	mpfr_init2(mp9, 128);
	mpfr_init2(mp10, 128);
	mpfr_init2(mp11, 128);
	mpfr_init2(mp12, 128);
	mpfr_init2(mp13, 128);
	mpfr_init2(mp14, 128);
	mpfr_init2(mp15, 128);
	mpfr_init2(mp16, 128);
	mpfr_init2(mp17, 128);
	mpfr_init2(mp18, 128);
	mpfr_set_d(mp1, -1, MPFR_RNDN);
	mpfr_set_d(mp2, 3.5, MPFR_RNDN);
	mpfr_mul(mp3, mp1, mp2, MPFR_RNDN);
	mpfr_set_d(mp4, 3.5, MPFR_RNDN);
	mpfr_set_d(mp5, 3.5, MPFR_RNDN);
	mpfr_mul(mp6, mp4, mp5, MPFR_RNDN);
	mpfr_set_d(mp7, 3, MPFR_RNDN);
	mpfr_set_d(mp8, c, MPFR_RNDN);
	mpfr_mul(mp9, mp7, mp8, MPFR_RNDN);
	mpfr_set_d(mp10, 4, MPFR_RNDN);
	mpfr_mul(mp11, mp9, mp10, MPFR_RNDN);
	mpfr_sub(mp12, mp6, mp11, MPFR_RNDN);
	mpfr_sqrt(mp13, mp12, MPFR_RNDN);
	mpfr_add(mp14, mp3, mp13, MPFR_RNDN);
	mpfr_set_d(mp15, 3, MPFR_RNDN);
	mpfr_set_d(mp16, 2, MPFR_RNDN);
	mpfr_mul(mp17, mp15, mp16, MPFR_RNDN);
	mpfr_div(mp18, mp14, mp17, MPFR_RNDN);

	mpfr_set(mpfrResult, mp18, MPFR_RNDN);
	mpfr_clear(mp1);
	mpfr_clear(mp2);
	mpfr_clear(mp3);
	mpfr_clear(mp4);
	mpfr_clear(mp5);
	mpfr_clear(mp6);
	mpfr_clear(mp7);
	mpfr_clear(mp8);
	mpfr_clear(mp9);
	mpfr_clear(mp10);
	mpfr_clear(mp11);
	mpfr_clear(mp12);
	mpfr_clear(mp13);
	mpfr_clear(mp14);
	mpfr_clear(mp15);
	mpfr_clear(mp16);
	mpfr_clear(mp17);
	mpfr_clear(mp18);
	return status;
}

int expr_smartRoot_2_mpfr(double c, mpfr_t mpfrResult) {
	if (c < 0.75) {
		expr_smartRoot_1_mpfr(c, mpfrResult);
	} else {
		expr_smartRoot_2_mpfr_old(c, mpfrResult);
	}
	return 0;
}