#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gmp.h>
#include <pthread.h>

#include "calc.h"

static inline void BBP_term(mpfr_t num, long long unsigned k, mpz_t aux[3]) {
    mpz_set_ui(aux[0], 120UL);
    mpz_mul_ui(aux[0], aux[0], k);
    mpz_add_ui(aux[0], aux[0], 151UL);
    mpz_mul_ui(aux[0], aux[0], k);
    mpz_add_ui(aux[0], aux[0], 47UL);

    mpz_set_ui(aux[1], 512UL);
    mpz_mul_ui(aux[1], aux[1], k);
    mpz_add_ui(aux[1], aux[1], 1024UL);
    mpz_mul_ui(aux[1], aux[1], k);
    mpz_add_ui(aux[1], aux[1], 712UL);
    mpz_mul_ui(aux[1], aux[1], k);
    mpz_add_ui(aux[1], aux[1], 194UL);
    mpz_mul_ui(aux[1], aux[1], k);
    mpz_add_ui(aux[1], aux[1], 15UL);

    mpz_ui_pow_ui(aux[2], 16UL, k);
    mpz_mul(aux[1], aux[1], aux[2]);

    mpfr_set_z(num, aux[0], MPFR_RNDN);
    mpfr_div_z(num, num, aux[1], MPFR_RNDN);
}

mpfr_t _err, *_ans;
long long unsigned _bits;
unsigned _threads;

static void *calc_pi_thread(void *initial_term) {
    mpz_t auxz[3];
    mpfr_t auxf;
    mpz_inits(auxz[0], auxz[1], auxz[2], NULL);
    mpfr_init2(auxf, _bits);

    unsigned th = *(unsigned *) initial_term;
    long long unsigned iter = th;
    do {
        BBP_term(auxf, iter, auxz);
        iter += _threads;

        mpfr_add(_ans[th], _ans[th], auxf, MPFR_RNDD);

    } while (mpfr_greaterequal_p(auxf, _err));

    mpfr_clear(auxf);
    mpz_clears(auxz[0], auxz[1], auxz[2], NULL);
}

void calc_pi(mpfr_t ans, long long digits, unsigned threads) {
    _threads = threads;
    _bits = 5 + (long long unsigned) \
        ceill(log2l(10.0) * (long double) digits);

    mpfr_init2(_err, _bits);

    mpfr_set_si(_err, -(digits+2), MPFR_RNDD);
    mpfr_exp10(_err, _err, MPFR_RNDD);

    _ans = malloc(threads * sizeof(mpfr_t));
    unsigned *inits = malloc(threads * sizeof(unsigned));
    for (unsigned i = 0; i < threads; i++) {
        inits[i] = i;
        mpfr_init2(_ans[i], _bits);
        mpfr_set_ui(_ans[i], 0UL, MPFR_RNDD);
    }

    pthread_t *ths = malloc(threads * sizeof(pthread_t));
    for (unsigned i = 0; i < threads; i++) {
        int ret = pthread_create(&(ths[i]), NULL, calc_pi_thread, &(inits[i]));
        if (ret != 0) {
            printf("Thread Fail: %d\n", ret);
        }
    }

    for (unsigned i = 0; i < threads; i++) {
        pthread_join(ths[i], NULL);
    }
    free(ths);
    free(inits);
    mpfr_clear(_err);

    mpfr_init2(ans, _bits);
    mpfr_set_ui(ans, 0UL, MPFR_RNDD);
    for (unsigned i = 0; i < threads; i++) {
        mpfr_add(ans, ans, _ans[i], MPFR_RNDD);
        mpfr_clear(_ans[i]);
    }
}

void print_pi(FILE *out, mpfr_t pi, long long digits) {
    mpfr_out_str(out, 10, digits, pi, MPFR_RNDD);
    fprintf(out, "\n");
    mpfr_clear(pi);
}
