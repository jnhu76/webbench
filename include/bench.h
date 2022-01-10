#ifndef _BENCH_H_
#define _BENCH_H_

#define VERSION 0.1

#include <string>
#include <iostream>

#include "httpRequest.h"


enum Method {
    GET = 1,
    HEAD = 2,
    OPTIONS = 3,
    TRACE = 4
};


class Bench {
public:
    Bench(std::string url, int clients = 1, int time = 30, Method m = GET)
        : url(url), clients(clients), time(time), method(m) {}

    ~Bench() = default;
    
    void print() {
        std::cout << url << "\t" << clients << "\t" << time << "\t" << method << "\n";
    }

private:
    std::string url;
    int clients;
    int time;
    Method method;
};



#endif  // _BENCH_H_