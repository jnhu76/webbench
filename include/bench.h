#ifndef _BENCH_H_
#define _BENCH_H_

#define VERSION 0.1

#include <string>
#include <thread>
#include <atomic>
#include <iostream>

#include "socks.h"


enum Method {
    GET = 1,
    HEAD = 2,
    OPTIONS = 3,
    TRACE = 4
};

volatile size_t timerexpired = 0;
std::atomic<size_t> speed {0};
std::atomic<size_t> failed {0};
std::atomic<size_t> bytes {0};

class Bench {
public:
    Bench(std::string url, int clients = 1, int time = 30, Method m = GET)
        : url(url), clients(clients), time(time), method(m) {}

    ~Bench() = default;
    
    void print() {
        std::cout << url << "\t" << clients << "\t" << time << "\t" << method << "\n";
    }

    void build_request(std::string url);

    int bench();

    void benchCore(std::string host, int port, std::string request);

private:
    std::string url;
    int clients;
    int time;
    Method method;
};



#endif  // _BENCH_H_