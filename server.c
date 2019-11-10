#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "voting.h"

int cycle = 1;
void SigintHandler(int sig)
{
    cycle = 0;
    printf("\nServer out\n");
    exit(0);
}

void save_sign(int_least64_t rnd, int_least64_t *s)
{
    int i;
    printf("save\n");
    FILE *fout = file_open("tmp/log", "a");
    fprintf(fout, "%"PRId64"\n", rnd);
    for(i = 0; i < LENGTH; i++)
        fprintf(fout, "%"PRId64" ", s[i]);
    fprintf(fout, "\n\n");
    fclose(fout);

}

int voting_allowed(int name)
{
    int tmp, flag_allow = 1;
    FILE *fin = file_open("tmp/allowed", "r");
    while(fscanf(fin, "%d", &tmp) != EOF)
    {
        if(tmp == name)
        {
            flag_allow = 0;
        }
    }
    fclose(fin);
    if(flag_allow)
    {
        FILE *fout = file_open("tmp/allowed", "a");
        fprintf(fout, "%d ", name);
        fclose(fout);
    }
    return flag_allow;
}

int main()
{
    struct sigaction sigint;
    sigint.sa_handler = SigintHandler;
    sigint.sa_flags = 0;
    sigemptyset(&sigint.sa_mask);
    sigaddset(&sigint.sa_mask, SIGINT);
    sigaction(SIGINT, &sigint, 0);
    
    int_least64_t n, c, d, rnd;
    int_least64_t h[LENGTH], s[LENGTH];
    int sock, child_sock, i, name, error_flag = 0;
    uint8_t digest[LENGTH];
    socklen_t size = 1;
    struct sockaddr_in addr, child;
    pid_t child_pid;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(-1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(-1);
    }
    listen(sock, 6);

    rsa_generate(&n, &c, &d);
    rsa_save_public_key(n, d);

    while(cycle)
    {
        child_sock = accept(sock, (struct sockaddr*)&child, &size);
        child_pid = fork();
        if(child_pid == 0)
        {
            send(child_sock, &n, sizeof(n), 0);
            send(child_sock, &d, sizeof(d), 0);

            recv(child_sock, &name, sizeof(name), 0);
            recv(child_sock, h, sizeof(h), 0);
            name = voting_allowed(name);
            send(child_sock, &name, sizeof(name), 0);
            if(!name)
                exit(-1);
            for(i = 0; i < LENGTH; i++)
                s[i] = modpow(h[i], c, n);
            send(child_sock, s, sizeof(s), 0);

            recv(child_sock, &rnd, sizeof(rnd), 0);
            recv(child_sock, s, sizeof(s), 0);

            hash_init();
            int2hash(&rnd, sizeof(rnd), digest);
            hash_finale(digest);
            for(i = 0; i < LENGTH; i++)
                if(digest[i] != modpow(s[i], d, n))
                {
                    printf("ERROR\n");
                    error_flag = 1;
                }
            if(!error_flag)
            {
                   save_sign(rnd, s);
            }
            exit(0);
        }
    }
    close(sock);
    exit(0);
}
