#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>


int Socket(const char* host, int clientPort) {
    unsigned long inaddr;
    struct sockaddr_in ad = {};
    
    ad.sin_family = AF_INET;
    inaddr = inet_addr(host);
    if (inaddr != INADDR_NONE) {
        ad.sin_addr.s_addr = inaddr;
    } else {
        auto hp = gethostbyname(host);
        if (hp == nullptr) {
            return -1;
        }
        memcpy(&ad.sin_addr, hp->h_addr_list[0], hp->h_length);
    }
    ad.sin_port = htons(clientPort);

    auto sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return sock;
    if (connect(sock, reinterpret_cast<struct sockaddr *>(&ad), sizeof(ad)) < 0)
        return -1;
    return sock;
}

#endif