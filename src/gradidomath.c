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

// conversion taken from: https://stackoverflow.com/questions/6248723/mpz-t-to-unsigned-long-long-conversion-gmp-lib

unsigned long long mpz_get_ull(mpz_ptr n, mpz_ptr tmp)
{
	unsigned long lo, hi;
	//mpz_t tmp;

	mpz_init(tmp);
	mpz_mod_2exp(tmp, n, 64);   /* tmp = (lower 64 bits of n) */

	lo = (unsigned long)mpz_get_ui(tmp);       /* lo = tmp & 0xffffffff */
	mpz_div_2exp(tmp, tmp, 32); /* tmp >>= 32 */
	hi = (unsigned long)mpz_get_ui(tmp);       /* hi = tmp & 0xffffffff */

	//mpz_clear(tmp);

	return (((unsigned long long)hi) << 32) + lo;
}

long long mpz_get_sll(mpz_ptr n, mpz_ptr tmp)
{
	return (long long)mpz_get_ull(n, tmp); /* just use unsigned version */
}

void mpz_set_sll(mpz_ptr n, long long sll)
{
	mpz_set_si(n, (int)(sll >> 32));     /* n = (int)sll >> 32 */
	mpz_mul_2exp(n, n, 32);             /* n <<= 32 */
	mpz_add_ui(n, n, (unsigned int)sll); /* n += (unsigned int)sll */
}

void mpz_set_ull(mpz_ptr n, unsigned long long ull)
{
	mpz_set_ui(n, (unsigned int)(ull >> 32)); /* n = (unsigned int)(ull >> 32) */
	mpz_mul_2exp(n, n, 32);                   /* n <<= 32 */
	mpz_add_ui(n, n, (unsigned int)ull);      /* n += (unsigned int)ull */
}



void calculateDecayFactor(mpfr_ptr decay_factor, int days_per_year)
{
	mpfr_t temp;
	mpfr_init(temp);
	// (lg Kn - lg K0) / seconds in year
	mpfr_log_ui(temp, 50, default_round);
	mpfr_log_ui(decay_factor, 100, default_round);
	mpfr_sub(temp, temp, decay_factor, default_round);
	
	mpfr_set_ui(decay_factor, days_per_year * 60 * 60 * 24, default_round);
	
	mpfr_div(decay_factor, temp, decay_factor, default_round);
	
	// precision error in advantage for user
	mpfr_exp(decay_factor, decay_factor, MPFR_RNDZ);
	
	mpfr_clear(temp);
	
}

void calculateDecayFactorForDuration(mpfr_ptr decay_for_duration, mpfr_ptr decay_factor, unsigned long seconds)
{
	mpfr_pow_ui(decay_for_duration, decay_factor, seconds, default_round);
}

void calculateDecayFast(mpfr_ptr decay_for_duration, mpfr_ptr gradido_decimal, mpz_ptr gradido_cent, mpfr_ptr temp)
{
	// (gradido_cent + gradido_decimal) * decay 

	mpfr_add_z(temp, gradido_decimal, gradido_cent, default_round);

	mpfr_mul(temp, temp, decay_for_duration, default_round);

	mpfr_get_z(gradido_cent, temp, MPFR_RNDZ);
	mpfr_sub_z(gradido_decimal, temp, gradido_cent, default_round);
	
}


GradidoWithDecimal calculateDecayForDuration(mpfr_ptr decay_for_duration, GradidoWithDecimal input)
{
	mpfr_t temp, gradido_decimal;
	mpz_t gdd_cent, z_temp;

	mpfr_init(temp); mpfr_init(gradido_decimal);
	mpz_init(gdd_cent); mpz_init(z_temp);

	mpfr_set_si(gradido_decimal, input.decimal, default_round);
	mpfr_div_si(gradido_decimal, gradido_decimal, GRADIDO_DECIMAL_CONVERSION_FACTOR, default_round);
	mpz_set_sll(gdd_cent, input.gradido);

	calculateDecayFast(decay_for_duration, gradido_decimal, gdd_cent, temp);

	GradidoWithDecimal result;
	result.gradido = mpz_get_sll(gdd_cent, z_temp);
	mpfr_mul_si(gradido_decimal, gradido_decimal, GRADIDO_DECIMAL_CONVERSION_FACTOR, default_round);
	result.decimal = mpfr_get_si(gradido_decimal, default_round);

	mpfr_clear(temp); mpfr_clear(gradido_decimal);
	mpz_clear(gdd_cent); mpz_clear(z_temp);

	return result;
}

GradidoWithDecimal calculateDecay(GradidoWithDecimal input, unsigned long seconds, mpfr_ptr decay_factor)
{
	mpfr_t decay_for_duration;
	mpfr_init(decay_for_duration);
	calculateDecayFactorForDuration(decay_for_duration, decay_factor, seconds);

	GradidoWithDecimal result = calculateDecayForDuration(decay_for_duration, input);

	mpfr_clear(decay_for_duration);

	return result;
}