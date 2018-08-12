#ifndef CALC_H
#define CALC_H

#include <mpfr.h>

void calc_pi(mpfr_t pi, long long digits, unsigned threads);
void print_pi(FILE *out, mpfr_t pi, long long digits);

#endif
