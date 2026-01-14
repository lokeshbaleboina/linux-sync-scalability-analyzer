#include <iostream>
#include <thread>
#include <vector>

static const long long ITERATIONS = 10'000'000;

void worker(long long& local_counter) {
    for (long long i = 0; i < ITERATIONS; i++) {
        local_counter++;
    }
}

int main(int argc, char* argv[]) {
    int num_threads = std::thread::hardware_concurrency();
    if (argc > 1) {
        num_threads = std::stoi(argv[1]);
    }

    std::vector<std::thread> threads;
    std::vector<long long> counters(num_threads, 0);

    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(worker, std::ref(counters[i]));
    }

    for (auto& t : threads) {
        t.join();
    }

    long long total = 0;
    for (auto c : counters) {
        total += c;
    }

    std::cout << "Final counter value: " << total << std::endl;
    return 0;
}
