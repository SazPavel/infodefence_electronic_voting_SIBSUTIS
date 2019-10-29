#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "voting.h"

int_least64_t generate_r(int_least64_t n)
{
    int_least64_t r;
    int_least64_t am[3], bm[3];
    while(1)
    {
        randombytes(&r, sizeof(r));
        r = fabs(r % 2147483646) + 1;
        am[0] = r; am[1] = 1; am[2] = 0;
        bm[0] = n; bm[1] = 0; bm[2] = 1;
        gcd_v(am, bm);
        if(am[0] == 1)
            break;
    }
    return r;
}

int_least64_t generate_rnd(int v)
{
    int_least64_t rnd;
    int i;
    randombytes(&rnd, sizeof(rnd));
    rnd = fabs(rnd % 2147483646) + 1;
    for(i = v; i != 0; i /= 10)
        rnd *= 10;
    rnd += v;
    return rnd;
}

int main(int argc, char *argv[])
{
    int_least64_t rnd, n, r, d, inr, temp;
    uint8_t digest[LENGTH];
    int_least64_t h[LENGTH], s[LENGTH];
    int v, i, name;
    int sock;
    struct sockaddr_in addr;
    if(argc < 2)
    {
        printf("example: ./client vote id\n");
        exit(0);
    }


    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("sock");
        exit(-1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sscanf(argv[1], "%d", &v);
    sscanf(argv[2], "%d", &name);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(-1);
    }

    recv(sock, &n, sizeof(n), 0);
    recv(sock, &d, sizeof(d), 0);

    //generate rnd
    rnd = generate_rnd(v);

    r = generate_r(n);

    //hash_function
    hash_init();
    int2hash(&rnd, sizeof(rnd), digest);
    hash_finale(digest);

    temp = modpow(r, d, n);
    for(i = 0; i < LENGTH; i++)
        h[i] = (digest[i] * temp) % n;

    send(sock, &name, sizeof(name), 0);
    send(sock, h, sizeof(h), 0);
    recv(sock, &name, sizeof(name), 0);
    if(!name)
    {
        printf("Only one vote!\n");
        exit(-1);
    }
    recv(sock, s, sizeof(s), 0);
    //6
    inversion_generate(n, r, &inr);
    for(i = 0; i < LENGTH; i++)
        s[i] = (s[i] * inr) % n;
    send(sock, &rnd, sizeof(rnd), 0);
    send(sock, s, sizeof(s), 0);
    close(sock);
    exit(0);
}
