#include "bench.h"
#include "cmdline.h"


// ================== Command line ==================
Bench command(int argc, char *argv[]) {
    cmdline::parser c;
    std::string url;
    Method m;
    bool http10 { true };

    c.add<int>("time", 't', "Run benchmark for <sec> seconds. Default 30.", false, 30);
    c.add<int>("clients", 'c', "Run <n> HTTP clients at once. Default 1.", false, 1);
    c.add("get", '\0', "Use GET request method.");
    c.add("head", '\0', "Use HEAD request method.");
    c.add("options", '\0', "Use OPTIONS request method.");
    c.add("trace", '\0', "Use TRACE request method.");
    c.add("http10", '\0', "Use http 1.0.");
    c.add("http11", '\0', "User http 1.1.");
    c.add("version", 'v', "Version.");

    c.footer(" URL ");
    c.parse_check(argc, argv);

    if (c.exist("version")) {
        std::cout << "WebBench " << VERSION << "\n";
        exit(0);
    }

    if ((c.exist("http11") + c.exist("http10")) > 1) {
        std::cerr << "HTTP Protocol error.\n";
        exit(0);
    } else
        if (c.exist("http11"))
            http10 = false;

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
    return Bench(url, c.get<int>("clients"), c.get<int>("time"), m, http10);
}

// ================== Bench Core ==================
//  处理 访问的url
void Bench::build_request() {

    switch (_method) {
        case GET: _request = "GET"; break;
        case HEAD: _request = "HEAD"; break;
        case OPTIONS: _request = "OPTIONS"; break;
        case TRACE: _request = "TRACE"; break;        
    }

    _request += " ";

    if (_url.size() > 1500) {
        std::cerr << "URL is too long.\n";
        exit(-1);
    }

    // check if url is valid.
    // http://www.zedwood.com/article/cpp-boost-url-regex
    std::regex url_regex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
    std::smatch url_match;
    std::string protocol, path, query;
    std::string port_tmp;
    if (std::regex_match(_url, url_match, url_regex)) {
        protocol    = std::string(url_match[1].first, url_match[1].second);
        _host      = std::string(url_match[2].first, url_match[2].second);
        port_tmp        = std::string(url_match[3].first, url_match[3].second);
        path        = std::string(url_match[4].first, url_match[4].second);
        query       = std::string(url_match[5].first, url_match[5].second);
    } else {
        std::cerr << "Invalid URL syntax.\n";
        exit(0);
    }
    if (port_tmp != "") {
        _port = stoi(port_tmp);
    } else {
        _port = 80;
    }

    _request += path + query;
    if (_http10)
        _request += " HTTP/1.0\r\n"; 
    else
        _request += " HTTP/1.1\r\n";
    std::string version = { VERSION.begin(), VERSION.end() };
    _request += "User-Agent: WebBench " + version + "\r\n";
    _request += "Host: " + _host + "\r\n";
    if (_force_reload)
        _request += "Pragma: no-cache\r\n";
    _request += "Connection: close\r\n";
    _request += "\r\n";
}

int bench(Bench& b) {

    // Check avaibility of target server.
    auto i = Socket(b.host(), b.port());
    if (i<0) {
        std::cerr << "\nConnect to server failed. Aborting benchmark.\n";
        return 1;
    }
    close(i);

    // create thread.
    std::vector<std::thread> ths;
    for (auto i=0;i<b.clients();i++) {
        std::thread th(&Bench::Run, &b);
        ths.emplace_back(std::move(th));
    }

    run = true;
    std::this_thread::sleep_for(std::chrono::seconds(b.time()));
    stop = true;
    for (auto &iter: ths) {
        iter.join();
    }
    // compute results.
    std::cout << "\nSpeed=" << (int)((speed+failed)/(b.time()/60.0f)) << " page/min, " << (int)(bytes/(float)b.time()) 
                << " bytes/sec.\nRequests: " << speed << " susceed, " << failed << " failed.\n";
    return i;
}

void Bench::Run() {
    if (!run) {
        std::this_thread::yield();
    }

    while (!stop) {
        benchCore();
    }
}

void Bench::benchCore() {
    int rlen;
    char buf[1500];
    
    rlen = _request.size();

    nexttry:while (true) {
        if (stop) break;
        auto s = Socket(_host, _port);
        if (s < 0) {
            failed++;
            continue;
        }
        if (rlen != write(s, _request.c_str(), rlen)) {
            failed++;
            close(s);
            continue;
        }

        if (!_force_reload) {
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
    bench(b);
    return 0;
}