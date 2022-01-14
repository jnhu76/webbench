#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


int Socket(const char* host, int clientPort) {
    unsigned long inaddr;
    struct sockaddr_in ad = {};
    
    ad.sin_family = AF_INET;
    inaddr = inet_addr(host);
    if (inaddr != INADDR_NONE) {
        ad.sin_addr.s_addr = inaddr;
    } else {
        struct hostent * hp = gethostbyname(host);
        if (hp == NULL) {
            return -1;
        }
        memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);
    }
    ad.sin_port = htons(clientPort);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return sock;
    if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0)
        return -1;
    return sock;
}

#endif