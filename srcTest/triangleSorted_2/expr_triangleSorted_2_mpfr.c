#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include <mpfr.h>

int expr_triangleSorted_1_mpfr(double a, double b, double c, mpfr_t mpfrResult) {
	int status = 0;
	mpfr_t mp1, mp2, mp3, mp4, mp5, mp6, mp7, mp8, mp9, mp10, mp11, mp12, mp13, mp14, mp15, mp16, mp17, mp18, mp19, mp20, mp21, mp22, mp23, mp24, mp25, mp26;
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
	mpfr_init2(mp19, 128);
	mpfr_init2(mp20, 128);
	mpfr_init2(mp21, 128);
	mpfr_init2(mp22, 128);
	mpfr_init2(mp23, 128);
	mpfr_init2(mp24, 128);
	mpfr_init2(mp25, 128);
	mpfr_init2(mp26, 128);
	mpfr_set_d(mp1, c, MPFR_RNDN);
	mpfr_set_d(mp2, b, MPFR_RNDN);
	mpfr_set_d(mp3, a, MPFR_RNDN);
	mpfr_add(mp4, mp2, mp3, MPFR_RNDN);
	mpfr_add(mp5, mp1, mp4, MPFR_RNDN);
	mpfr_set_d(mp6, a, MPFR_RNDN);
	mpfr_set_d(mp7, c, MPFR_RNDN);
	mpfr_set_d(mp8, b, MPFR_RNDN);
	mpfr_sub(mp9, mp7, mp8, MPFR_RNDN);
	mpfr_sub(mp10, mp6, mp9, MPFR_RNDN);
	mpfr_mul(mp11, mp5, mp10, MPFR_RNDN);
	mpfr_set_d(mp12, a, MPFR_RNDN);
	mpfr_set_d(mp13, c, MPFR_RNDN);
	mpfr_set_d(mp14, b, MPFR_RNDN);
	mpfr_sub(mp15, mp13, mp14, MPFR_RNDN);
	mpfr_add(mp16, mp12, mp15, MPFR_RNDN);
	mpfr_mul(mp17, mp11, mp16, MPFR_RNDN);
	mpfr_set_d(mp18, c, MPFR_RNDN);
	mpfr_set_d(mp19, b, MPFR_RNDN);
	mpfr_set_d(mp20, a, MPFR_RNDN);
	mpfr_sub(mp21, mp19, mp20, MPFR_RNDN);
	mpfr_add(mp22, mp18, mp21, MPFR_RNDN);
	mpfr_mul(mp23, mp17, mp22, MPFR_RNDN);
	mpfr_sqrt(mp24, mp23, MPFR_RNDN);
	mpfr_set_d(mp25, 4, MPFR_RNDN);
	mpfr_div(mp26, mp24, mp25, MPFR_RNDN);

	mpfr_set(mpfrResult, mp26, MPFR_RNDN);
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
	mpfr_clear(mp19);
	mpfr_clear(mp20);
	mpfr_clear(mp21);
	mpfr_clear(mp22);
	mpfr_clear(mp23);
	mpfr_clear(mp24);
	mpfr_clear(mp25);
	mpfr_clear(mp26);
	return status;
}

int expr_triangleSorted_2_mpfr_old(double a, double b, double c, mpfr_t mpfrResult) {
	int status = 0;
	mpfr_t mp1, mp2, mp3, mp4, mp5, mp6, mp7, mp8, mp9, mp10, mp11, mp12, mp13, mp14, mp15, mp16, mp17, mp18, mp19, mp20, mp21, mp22, mp23, mp24, mp25, mp26;
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
	mpfr_init2(mp19, 128);
	mpfr_init2(mp20, 128);
	mpfr_init2(mp21, 128);
	mpfr_init2(mp22, 128);
	mpfr_init2(mp23, 128);
	mpfr_init2(mp24, 128);
	mpfr_init2(mp25, 128);
	mpfr_init2(mp26, 128);
	mpfr_set_d(mp1, c, MPFR_RNDN);
	mpfr_set_d(mp2, a, MPFR_RNDN);
	mpfr_set_d(mp3, b, MPFR_RNDN);
	mpfr_add(mp4, mp2, mp3, MPFR_RNDN);
	mpfr_add(mp5, mp1, mp4, MPFR_RNDN);
	mpfr_set_d(mp6, b, MPFR_RNDN);
	mpfr_set_d(mp7, c, MPFR_RNDN);
	mpfr_set_d(mp8, a, MPFR_RNDN);
	mpfr_sub(mp9, mp7, mp8, MPFR_RNDN);
	mpfr_sub(mp10, mp6, mp9, MPFR_RNDN);
	mpfr_mul(mp11, mp5, mp10, MPFR_RNDN);
	mpfr_set_d(mp12, b, MPFR_RNDN);
	mpfr_set_d(mp13, c, MPFR_RNDN);
	mpfr_set_d(mp14, a, MPFR_RNDN);
	mpfr_sub(mp15, mp13, mp14, MPFR_RNDN);
	mpfr_add(mp16, mp12, mp15, MPFR_RNDN);
	mpfr_mul(mp17, mp11, mp16, MPFR_RNDN);
	mpfr_set_d(mp18, c, MPFR_RNDN);
	mpfr_set_d(mp19, a, MPFR_RNDN);
	mpfr_set_d(mp20, b, MPFR_RNDN);
	mpfr_sub(mp21, mp19, mp20, MPFR_RNDN);
	mpfr_add(mp22, mp18, mp21, MPFR_RNDN);
	mpfr_mul(mp23, mp17, mp22, MPFR_RNDN);
	mpfr_sqrt(mp24, mp23, MPFR_RNDN);
	mpfr_set_d(mp25, 4, MPFR_RNDN);
	mpfr_div(mp26, mp24, mp25, MPFR_RNDN);

	mpfr_set(mpfrResult, mp26, MPFR_RNDN);
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
	mpfr_clear(mp19);
	mpfr_clear(mp20);
	mpfr_clear(mp21);
	mpfr_clear(mp22);
	mpfr_clear(mp23);
	mpfr_clear(mp24);
	mpfr_clear(mp25);
	mpfr_clear(mp26);
	return status;
}

int expr_triangleSorted_2_mpfr(double a, double b, double c, mpfr_t mpfrResult) {
	if (a < b) {
		expr_triangleSorted_2_mpfr_old(a, b, c, mpfrResult);
	} else {
		expr_triangleSorted_1_mpfr(a, b, c, mpfrResult);
	}
}