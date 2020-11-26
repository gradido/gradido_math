#include "gradidomath.h"


/*
typedef enum {
MPFR_RNDN=0,  // round to nearest, with ties to even
MPFR_RNDZ,    // round toward zero
MPFR_RNDU,    // round toward +Inf
MPFR_RNDD,    // round toward -Inf
MPFR_RNDA,    // round away from zero
MPFR_RNDF,    // faithful rounding
MPFR_RNDNA = -1 // round to nearest, with ties away from zero (mpfr_round)
} mpfr_rnd_t;

*/
static const mpfr_rnd_t default_round = MPFR_RNDN;

void calculateDecayFactor(mpfr_ptr decay_factor, int days_per_year)
{
	mpfr_t temp;
	mpfr_init(temp);
	printf("after temp init\n");
	// (lg Kn - lg K0) / seconds in year
	mpfr_log_ui(temp, 50, default_round);
	printf("after lg Kn\n");
	mpfr_log_ui(decay_factor, 100, default_round);
	printf("after lg K0\n");
	mpfr_sub(temp, temp, decay_factor, default_round);
	printf("after lg Kn - lg K0\n");
	mpfr_set_ui(decay_factor, days_per_year * 60 * 60 * 24, default_round);
	printf("after set days per_second\n");
	mpfr_div(decay_factor, temp, decay_factor, default_round);
	printf("after div \n");
	// precision error in advantage for user
	mpfr_exp(decay_factor, decay_factor, MPFR_RNDZ);
	printf("after exp\n");
	mpfr_clear(temp);

}

void calculateDecayFactorForDuration(mpfr_ptr decay_for_duration, mpfr_ptr decay_factor, unsigned long seconds)
{
	mpfr_pow_ui(decay_for_duration, decay_factor, seconds, default_round);
}

long calculateDecay(mpfr_ptr decay_for_duration, mpfr_ptr gradido_decimal, long gradido_cent, mpfr_ptr temp)
{
	// (gradido_cent + gradido_decimal) * decay 
	mpfr_add_si(temp, gradido_decimal, gradido_cent, default_round);
	mpfr_mul(temp, temp, decay_for_duration, default_round);
	long gradido_decay_cent = mpfr_get_si(temp, MPFR_RNDZ);
	mpfr_sub_si(gradido_decimal, temp, gradido_decay_cent, default_round);
	// result without gradido_decimal
	return gradido_cent - gradido_decay_cent;
}

long calculateDecayForDuration(mpfr_ptr decay_factor, mpfr_ptr gradido_decimal, long gradido_cent, unsigned long seconds, mpfr_ptr temp, mpfr_ptr temp2)
{
	calculateDecayFactorForDuration(temp2, decay_factor, seconds);
	return calculateDecay(temp2, gradido_decimal, gradido_cent, temp);
}

long calculateDecayForDurationWithoutTemp(mpfr_ptr decay_factor, mpfr_ptr gradido_decimal, long gradido_cent, unsigned long seconds)
{
	mpfr_t temp, temp2;
	mpfr_init(temp); mpfr_init(temp2);

	calculateDecayFactorForDuration(temp2, decay_factor, seconds);
	auto result = calculateDecay(temp2, gradido_decimal, gradido_cent, temp);

	mpfr_clear(temp); mpfr_clear(temp2);
	return result;
}