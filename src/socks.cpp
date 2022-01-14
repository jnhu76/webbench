#include "socks.h"

int Socket(std::string host, int clientPort) {
    unsigned long inaddr;
    struct sockaddr_in ad = {};
    
    ad.sin_family = AF_INET;
    inaddr = inet_addr(host.c_str());
    if (inaddr != INADDR_NONE) {
        ad.sin_addr.s_addr = inaddr;
    } else {
        struct hostent * hp = gethostbyname(host.c_str());
        if (hp == nullptr) {
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