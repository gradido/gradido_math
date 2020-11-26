
#include "gradidomath.h"
#include "gtest/gtest.h"

#include <chrono>

TEST(gradido_math, calculate_decay_factor)
{
	mpfr_t decay_365, decay_366, temp;
	printf("after variable declaration\n");
	mpfr_init(decay_365); mpfr_init(decay_366); mpfr_init(temp);
	printf("after variablen initialization\n");

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	printf("after clock begin\n");
	calculateDecayFactor(decay_365, 365);
	printf("after first decay\n");
	calculateDecayFactor(decay_366, 366);
	printf("after second decay\n");
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	printf("after clock end\n");

	std::cout << "duration = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << " [ns], ";
	std::cout << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " [micro s]" << std::endl;

	// print string
	mpfr_exp_t exp_temp;
	char* str_decay = mpfr_get_str(NULL, &exp_temp, 10, 0, decay_365, MPFR_RNDN);
	printf("decay rate per second for 365 days: 0.%s\n", str_decay);
	mpfr_free_str(str_decay);

	str_decay = mpfr_get_str(NULL, &exp_temp, 10, 0, decay_366, MPFR_RNDN);
	printf("decay rate per second for 366 days: 0.%s\n", str_decay);
	mpfr_free_str(str_decay);

	mpfr_clear(decay_365); mpfr_clear(decay_366); mpfr_clear(temp);

}

TEST(gradido_math, calculate_decay_factor_for_duration)
{
	mpfr_t decay_factor, temp;
	mpfr_init(decay_factor); mpfr_init(temp);

	calculateDecayFactor(decay_factor, 365);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
	calculateDecayFactorForDuration(decay_factor, decay_factor, 60*60*24*365);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "duration = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << " [ns], ";
	std::cout << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " [micro s]" << std::endl;

	// print string
	mpfr_exp_t exp_temp;
	char* str_decay = mpfr_get_str(NULL, &exp_temp, 10, 0, decay_factor, MPFR_RNDN);
	printf("decay factor for 1 day (365 days in year): 0.%s\n", str_decay);
	mpfr_free_str(str_decay);

	mpfr_clear(decay_factor); mpfr_clear(temp);
}

TEST(gradido_math, calculate_decay)
{
	mpfr_t decay_factor, gradido_decimal, temp;
	mpfr_init(decay_factor); mpfr_init_set_si(gradido_decimal, 0, MPFR_RNDN); mpfr_init(temp);

	calculateDecayFactor(decay_factor, 365);
	calculateDecayFactorForDuration(decay_factor, decay_factor, 60 * 60 * 24 * 365);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	auto cent_after = calculateDecay(decay_factor, gradido_decimal, 1000000, temp);
	
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "duration = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << " [ns], ";
	std::cout << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " [micro s]" << std::endl;

	// print string
	mpfr_exp_t exp_temp;
	char* str_decay = mpfr_get_str(NULL, &exp_temp, 10, 0, gradido_decimal, MPFR_RNDN);
	printf("gradido_decimal: 0.%s\n", str_decay);
	mpfr_free_str(str_decay);

	printf("gradido cent after: %d\n", cent_after);
	mpfr_clear(decay_factor); mpfr_clear(gradido_decimal); mpfr_clear(temp);
}

TEST(gradido_math, calculate_decay_for_duration)
{
	mpfr_t decay_factor, gradido_decimal, temp, temp2;
	mpfr_init(decay_factor); mpfr_init_set_si(gradido_decimal, 0, MPFR_RNDN); mpfr_init(temp); mpfr_init(temp2);

	calculateDecayFactor(decay_factor, 365);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	auto cent_after = calculateDecayForDuration(decay_factor, gradido_decimal, 1000000, 60 * 60 * 24 * 365, temp, temp2);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "duration = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << " [ns], ";
	std::cout << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " [micro s]" << std::endl;

	// print string
	mpfr_exp_t exp_temp;
	char* str_decay = mpfr_get_str(NULL, &exp_temp, 10, 0, gradido_decimal, MPFR_RNDN);
	printf("gradido_decimal: 0.%s\n", str_decay);
	mpfr_free_str(str_decay);

	printf("gradido cent after: %d\n", cent_after);
	mpfr_clear(decay_factor); mpfr_clear(gradido_decimal); mpfr_clear(temp); mpfr_clear(temp2);
}

TEST(gradido_math, calculate_decay_for_duration_without_temp)
{
	mpfr_t decay_factor, gradido_decimal;
	mpfr_init(decay_factor); mpfr_init_set_si(gradido_decimal, 0, MPFR_RNDN);

	calculateDecayFactor(decay_factor, 365);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	auto cent_after = calculateDecayForDurationWithoutTemp(decay_factor, gradido_decimal, 1000000, 60 * 60 * 24 * 365);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "duration = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << " [ns], ";
	std::cout << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " [micro s]" << std::endl;

	// print string
	mpfr_exp_t exp_temp;
	char* str_decay = mpfr_get_str(NULL, &exp_temp, 10, 0, gradido_decimal, MPFR_RNDN);
	printf("gradido_decimal: 0.%s\n", str_decay);
	mpfr_free_str(str_decay);

	printf("gradido cent after: %d\n", cent_after);
	mpfr_clear(decay_factor); mpfr_clear(gradido_decimal);
}