#ifndef VOTING_H
#define VOTING_H
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include "ciphers.h"
#include "infodef.h"

#if HASH==256
#define LENGTH SHA256_DIGEST_LENGTH
SHA256_CTX hash;
#elif HASH==224
#define LENGTH SHA224_DIGEST_LENGTH
SHA256_CTX hash;
#elif HASH==512
#define LENGTH SHA512_DIGEST_LENGTH
SHA512_CTX hash;
#elif HASH==384
#define LENGTH SHA384_DIGEST_LENGTH
SHA512_CTX hash;
#else
#define LENGTH MD5_DIGEST_LENGTH
MD5_CTX hash;
#endif

void hash_init()
{
#if HASH==256
    SHA256_Init(&hash);
#elif HASH==224
    SHA224_Init(&hash);
#elif HASH==512
    SHA512_Init(&hash);
#elif HASH==384
    SHA384_Init(&hash);
#else
    MD5_Init(&hash);
#endif
}

void int2hash(int_least64_t *num, int length, uint8_t *digest)
{
#if HASH==256
    SHA256_Update(&hash, num, length);
#elif HASH==224
    SHA224_Update(&hash, num, length);
#elif HASH==512
    SHA512_Update(&hash, num, length);
#elif HASH==384
    SHA384_Update(&hash, num, length);
#else
    MD5_Update(&hash, num, length);
#endif
}

void hash_finale(uint8_t *digest){
#if HASH==256
    SHA256_Final(digest, &hash);
#elif HASH==224
    SHA224_Final(digest, &hash);
#elif HASH==512
    SHA512_Final(digest, &hash);
#elif HASH==384
    SHA384_Final(digest, &hash);
#else
    MD5_Final(digest, &hash);
#endif
}

void inversion_generate(int_least64_t p, int_least64_t c, int_least64_t *d)
{
    int_least64_t am[3], bm[3];
    do
    {
        am[0] = p; am[1] = 1; am[2] = 0;
        bm[0] = c; bm[1] = 0; bm[2] = 1;
        gcd_v(am, bm);
    }while(am[0] != 1);
    if(am[2] < 0)
        am[2] += p;
    *d = am[2];
}

#endif
