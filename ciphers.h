#ifndef ciphers_H
#define ciphers_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <inttypes.h>
#include "randombytes.h"
#include "infodef.h"


FILE *file_open(char *name, char *mode);

void lgamal_generate_xy(int_least64_t p, int_least64_t g, int_least64_t *x, int_least64_t *y);
void lgamal_encrypt(int_least64_t g, int_least64_t p, int_least64_t y, char *in, char *out);
void lgamal_decrypt(int_least64_t p, int_least64_t x, char *in, char *out);
void lgamal_save_public_key(int_least64_t p, int_least64_t y, int_least64_t g);
void lgamal_save_private_key(int_least64_t p, int_least64_t x);
void lgamal_load_public_key(int_least64_t *p, int_least64_t *y, int_least64_t *g);
void lgamal_load_private_key(int_least64_t *p, int_least64_t *x);

void rsa_generate(int_least64_t *n, int_least64_t *c, int_least64_t *d);
void rsa_encrypt(int_least64_t n, int_least64_t d, char *in, char *out);
void rsa_decrypt(int_least64_t n, int_least64_t c, char *in, char *out);
void rsa_save_public_key(int_least64_t n, int_least64_t d);
void rsa_save_private_key(int_least64_t n, int_least64_t c);
void rsa_load_public_key(int_least64_t *n, int_least64_t *d);
void rsa_load_private_key(int_least64_t *n, int_least64_t *c);

void sham_generate(int_least64_t p, int_least64_t *c, int_least64_t *d);
void sham_first_en(int_least64_t p, int_least64_t ca, char *in, char *out);
void sham_temp_crypt(int_least64_t p, int_least64_t b, char *in, char *out);
void sham_second_de(int_least64_t p, int_least64_t db, char *in, char *out);
void sham_save_key(int_least64_t c, int_least64_t d, char *filename);
void sham_save_prime(int_least64_t p);
void sham_load_key(int_least64_t *c, int_least64_t *d, char *filename);
void sham_load_prime(int_least64_t *p);

char* vernam_encrypt(char *in, char *out);
void vernam_decrypt(char *key, char *in, char *out);
char* vernam_load_key();
#endif 
