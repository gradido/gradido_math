#ifndef _GRADIDO_MATH_H
#define _GRADIDO_MATH_H

#include <mpfr.h>

#ifdef __cplusplus
extern "C" {
#endif


void calculateDecayFactor(mpfr_ptr decay_factor, int days_per_year);
void calculateDecayFactorForDuration(mpfr_ptr decay_for_duration, mpfr_ptr decay_factor, unsigned long seconds);

long calculateDecay(mpfr_ptr decay_for_duration, mpfr_ptr gradido_decimal, long gradido_cent, mpfr_ptr temp);
long calculateDecayForDuration(mpfr_ptr decay_factor, mpfr_ptr gradido_decimal, long gradido_cent, unsigned long seconds, mpfr_ptr temp, mpfr_ptr temp2);
long calculateDecayForDurationWithoutTemp(mpfr_ptr decay_factor, mpfr_ptr gradido_decimal, long gradido_cent, unsigned long seconds);

#ifdef __cplusplus
}
#endif

#endif //_GRADIDO_MATH_H