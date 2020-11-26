#ifndef _GRADIDO_MATH_H
#define _GRADIDO_MATH_H

#include <mpfr.h>

#ifdef __cplusplus
extern "C" {
#endif

//! \param decay_factor ptr to store the result
//! \param days_per_year 365 days in normal year, 366 days in leap year
//! \brief calculate decay factor per second for exponential decay calculation, needs 100-400 micro seconds depending on processor
/*!
 *  calculate decay factor with compound interest formula converted to q
 *  n = (lg Kn - lg K0) / lg q => <br>
 *  lg q = (lg Kn - lg K0) / n => <br>
 *  q = e^((lg Kn - lg K0) / n)   <br>
 * <br>
 * with:
 * <ul>
 *  <li>q = decay_factor</li>
 *  <li>n = days_per_year * 60 * 60 * 24 = seconds per year</li>
 *  <li>Kn = 50 (capital after a year)</li>
 *  <li>K0 = 100 (capital at start)</li>
 * </ul>
 * 
*/
void calculateDecayFactor(mpfr_ptr decay_factor, int days_per_year);
void calculateDecayFactorForDuration(mpfr_ptr decay_for_duration, mpfr_ptr decay_factor, unsigned long seconds);

long calculateDecay(mpfr_ptr decay_for_duration, mpfr_ptr gradido_decimal, long gradido_cent, mpfr_ptr temp);
long calculateDecayForDuration(mpfr_ptr decay_factor, mpfr_ptr gradido_decimal, long gradido_cent, unsigned long seconds, mpfr_ptr temp, mpfr_ptr temp2);
long calculateDecayForDurationWithoutTemp(mpfr_ptr decay_factor, mpfr_ptr gradido_decimal, long gradido_cent, unsigned long seconds);

#ifdef __cplusplus
}
#endif

#endif //_GRADIDO_MATH_H