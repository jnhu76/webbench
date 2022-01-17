#include "bench.h"
#include "cmdline.h"


// ================== Command line ==================
Bench command(int argc, char *argv[]) {
    cmdline::parser c;
    std::string url;
    Method m;

    c.add<int>("time", 't', "Run benchmark for <sec> seconds. Default 30.", false, 30);
    c.add<int>("clients", 'c', "Run <n> HTTP clients at once. Default 1.", false, 1);
    c.add("get", '\0', "Use GET request method.");
    c.add("head", '\0', "Use HEAD request method.");
    c.add("options", '\0', "Use OPTIONS request method.");
    c.add("trace", '\0', "Use TRACE request method.");
    c.add("version", 'v', "Version.");

    c.footer(" URL ");
    c.parse_check(argc, argv);

    if (c.exist("version")) {
        std::cout << "WebBench " << VERSION << "\n";
        exit(0);
    }

    if ((c.exist("trace") + c.exist("options") + c.exist("head") + c.exist("get")) > 1) {
        std::cerr << "Method error.\n";
        exit(0);
    } else {
        if (c.exist("trace"))
            m = TRACE;
        else if (c.exist("options"))
            m = OPTIONS;
        else if (c.exist("head"))
            m = HEAD;
        else
            m = GET;
    }

    if (c.rest().size() == 1) {
        url = c.rest()[0];
    } else {
        std::cerr << "Please input url.\n";
        exit(0);
    }

    // Bench b = {url, c.get<int>("clients"), c.get<int>("time"), m};
    // return b;
    // Warnning: moving a temporary object prevents copy elision.
    // return std::move(Bench(url, c.get<int>("clients"), c.get<int>("time"), m));
    return Bench(url, c.get<int>("clients"), c.get<int>("time"), m);
}

// ================== Bench Core ==================
//  处理 访问的url
void Bench::build_request() {

    switch (method) {
        case GET: request = "GET"; break;
        case HEAD: request = "HEAD"; break;
        case OPTIONS: request = "OPTIONS"; break;
        case TRACE: request = "TRACE"; break;        
    }

    request += " ";

    if (url.size() > 1500) {
        std::cerr << "URL is too long.\n";
        exit(-1);
    }

    // check if url is valid.
    // http://www.zedwood.com/article/cpp-boost-url-regex
    std::regex url_regex("(http)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
    std::smatch url_match;
    std::string protocol, path, query;
    std::string port_tmp;
    if (std::regex_match(url, url_match, url_regex)) {
        protocol    = std::string(url_match[1].first, url_match[1].second);
        host      = std::string(url_match[2].first, url_match[2].second);
        port_tmp        = std::string(url_match[3].first, url_match[3].second);
        path        = std::string(url_match[4].first, url_match[4].second);
        query       = std::string(url_match[5].first, url_match[5].second);
    } else {
        std::cerr << "Invalid URL syntax.\n";
        exit(0);
    }
    if (port_tmp != "") {
        port = stoi(port_tmp);
    } else {
        port = 80;
    }

    request += path + query;
    if (http10 == 1)
        request += " HTTP/1.1\r\n"; 
    else
        request += " HTTP/1.0\r\n";
    std::string version = { VERSION.begin(), VERSION.end() };
    request += "User-Agent: WebBench " + version + "\r\n";
    request += "Host: " + host;
    if (force_reload)
        request += "Pragma: no-cache\r\n";
    if (http10>1)
        request += "Connection: close\r\n";

    request += "\r\n";
}

int Bench::bench() {

    // Check avaibility of target server.
    auto i = Socket(host, port);
    if (i<0) {
        std::cerr << "\nConnect to server failed. Aborting benchmark.\n";
        return 1;
    }
    close(i);

    // create thread.
    std::vector<std::unique_ptr<std::thread>> ths;
    std::cout << "start threads: " << "\n";
    for (auto i=0;i<clients;i++) {
        ths.emplace_back(std::make_unique<std::thread>([&]() {
            Run();
        }));
    }
    std::this_thread::sleep_for(std::chrono::seconds(time));

    stop = true;
    std::cout << "start threads: " << "\n";
    for (auto &iter: ths) {
        iter->join();
    }

    // compute results.
    std::cout << "\nSpeed=" << (int)((speed+failed)/(time/60.0f)) << " page/min, " << (int)(bytes/(float)time) 
                << ", bytes/sec.\nRequests: " << speed << " susceed, " << failed << " failed.\n";
    return i;
}

void Bench::Run() {
    // while (!run) {
    //     std::this_thread::yield();
    // }

    while (!stop) {
        std::cout << "bencore\n";
        benchCore();
    }
}

void Bench::benchCore() {
    int rlen;
    char buf[1500];
    
    rlen = request.size();

    nexttry:while (true) {
        auto s = Socket(host, port);
        if (s < 0) {
            failed++;
            continue;
        }
        if (rlen != write(s, request.c_str(), rlen)) {
            failed++;
            close(s);
            continue;
        }

        if (!force_reload) {
            while (true) {
                auto i = read(s, buf, 1500);
                if (i<0) {
                    failed++;
                    close(s);
                    goto nexttry;
                } else {
                    if (i == 0)
                        break;
                    else
                        bytes += i;
                }
            }
        }
        if (close(s)) {
            failed++;
            continue;
        }
        speed++;
    }
}

int main(int argc, char *argv[]) {
    static_assert(0 == 0);
    auto b = command(argc, argv);
    b.print();
    b.bench();
    return 0;
}