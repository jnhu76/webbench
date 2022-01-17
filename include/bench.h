#ifndef _BENCH_H_
#define _BENCH_H_

#include <string>
#include <thread>
#include <atomic>
#include <regex>
#include <chrono>
#include <vector>
#include <memory>
#include <iostream>

#include "socks.h"

constexpr std::string_view VERSION {"0.1"};

enum Method {
    GET = 1,
    HEAD = 2,
    OPTIONS = 3,
    TRACE = 4
};

volatile size_t timerexpired = 0;


class Bench {
public:
    Bench(std::string &url, int clients = 1, int time = 30, Method m = GET)
        : url(url), clients(clients), time(time), method(m) {
            build_request();
        }

    ~Bench() = default;
    
    void print() {
        std::cout << url << "\t" << clients << "\t" << time << "\t" << method << "\n";
        std::cout << "requests: " << request << "\n";
    }

    void build_request();

    int bench();

    void Run();

    void benchCore();

private:

    // 获取的参数
    std::string url;
    int clients;
    int time;
    Method method;
    bool force_reload { false };
    size_t http10 { 1 }; // 1 == http1.0, 2 == http.1.1;
    
    // 解析 url
    std::string request;
    std::string host;
    int port;

    // 结果
    std::atomic<size_t> speed {0};
    std::atomic<size_t> failed {0};
    std::atomic<size_t> bytes {0};

    // 控制线程
    std::atomic<bool> run {false};
    std::atomic<bool> stop {false};
};



#endif  // _BENCH_H_