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
std::atomic<bool> run {false};
std::atomic<bool> stop {false};

std::atomic<size_t> speed {0};
std::atomic<size_t> failed {0};
std::atomic<size_t> bytes {0};

class Bench {
public:
    Bench(std::string &url, int clients = 1, int time = 30, Method m = GET, bool h = true)
        : _url(url), _clients(clients), _time(time), _method(m), _http10(h) {
            build_request();
        }

    ~Bench() = default;
    
    void print() {
        std::cout << "Bench params: \n  url: " << _url << "\n  threads: "
            << _clients << "\n  times: " << _time << "s.\n";
        std::cout << "requests:\n" << _request << "\n";
    }

    // getter.
    int clients() const { return _clients; }
    const std::string url() const { return _url; }
    int time() const { return _time; }
    Method method() const { return _method; }
    bool force_reload() const { return _force_reload; }
    size_t http10() const { return _http10; }
    const std::string request() const { return _request; }
    const char* host() const { return _host.c_str(); }
    int port() const { return _port; }

    void build_request();

    void Run();

    void benchCore();

private:

    // 获取的参数
    std::string _url;
    int _clients;
    int _time;
    Method _method;
    bool _force_reload { true };
    bool _http10 { true }; // true == http1.0, false == http.1.1;
    
    // 解析 url
    std::string _request;
    std::string _host;
    int _port;
};

#endif  // _BENCH_H_