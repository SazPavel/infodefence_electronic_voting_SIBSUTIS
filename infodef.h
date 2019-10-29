#ifndef infodef_H
#define infodef_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include "randombytes.h"

struct BabyStep{
    int_least64_t value;
    int_least64_t index;
};

struct vector{
    int_least64_t e[3]; 
};

int_least64_t jacobi(int_least64_t a, int_least64_t b);
int bpsw (int_least64_t n);
int_least64_t high_bit_bs(int_least64_t n);
int miller_rabin(int_least64_t n, int k);
int prime_test(int_least64_t n);
int_least64_t diff_hell_man(int_least64_t p, int_least64_t q);
uint_least64_t modpow(int_least64_t base, int_least64_t exponent, int_least64_t module);
void gcd_v(int_least64_t *a, int_least64_t *b);
int comp_baby_step(const void *ptr1, const void *ptr2);
int_least64_t shencs(int_least64_t base, int_least64_t module, int_least64_t result);
void prime_safe_generate(int_least64_t *p, int_least64_t *g, int_least64_t max, int_least64_t min);

#endif 
