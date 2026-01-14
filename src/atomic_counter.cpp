#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

static const long long ITERATIONS = 10'000'000;

std::atomic<long long> counter(0);

void worker() {
    for (long long i = 0; i < ITERATIONS; i++) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main(int argc, char* argv[]) {
    int num_threads = std::thread::hardware_concurrency();
    if (argc > 1) {
        num_threads = std::stoi(argv[1]);
    }

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(worker);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final counter value: " << counter.load() << std::endl;
    return 0;
}
