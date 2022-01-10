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
        std::cout << "Method error.\n";
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
        std::cout << "Please input url.\n";
        exit(0);
    }

    // Bench b = {url, c.get<int>("clients"), c.get<int>("time"), m};
    // return b;
    // Warnning: moving a temporary object prevents copy elision.
    // return std::move(Bench(url, c.get<int>("clients"), c.get<int>("time"), m));
    return Bench(url, c.get<int>("clients"), c.get<int>("time"), m);
}

int main(int argc, char *argv[]) {
    static_assert(0 == 0);
    auto b = command(argc, argv);
    b.print();
    return 0;
}