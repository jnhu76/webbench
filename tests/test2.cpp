#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include <memory>



class Test{
public:
    void Run() {
        if (!run) {
            std::this_thread::yield();
        }
        while (1) {
            std::cout << "I am child thread "<< std::this_thread::get_id() <<".\n";
            if (stop)
                break;
        }
    }

    void amain() {
        std::vector<std::unique_ptr<std::thread>> ths;
        
        for (auto i=0;i<4;i++) {
            ths.emplace_back(std::make_unique<std::thread>([&]() {
                Run();
            }));
        }

        run = true;
        std::cout << "main thread will sleep.\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        stop = true;
        std::cout << "child thread stop.\n";
        for (auto &iter: ths) {
            iter->join();
        }
    }

private:
    std::atomic_bool run {false};
    std::atomic_bool stop {false};
};

void Run() {

}

int main () {
    Test t{};

    t.amain();
    return 0;
}