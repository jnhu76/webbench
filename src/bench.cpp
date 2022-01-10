#include "bench.h"

[[maybe_unused]] void foo() {
    std::cout << "hello world" << "\n";
}

int main() {
    static_assert(0 == 0);
    std::cout << "Hello, c++2a" << "\n";
    return 0;
}