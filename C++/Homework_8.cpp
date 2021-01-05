#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

std::mutex m;
std::condition_variable cond1;
std::condition_variable cond2;
static const int max_size = 50;
std::atomic<bool> isReady(false);

enum PingPong{
    ping,
    pong,
};

void process(const PingPong s) {
    for (int i = 0; i < max_size; ++i) {
        if (s == pong) {
            std::unique_lock<std::mutex> lock(m);
            while (!isReady)
                cond1.wait(lock);
            isReady = false;
            std::cout << "pong" << '\n';
            cond2.notify_one();
        } else {
            std::unique_lock<std::mutex> lock(m);
            while (isReady)
                cond2.wait(lock);
            isReady = true;
            std::cout << "ping" << '\n';
            cond1.notify_one();
        }
    }
}

int main() {
    PingPong s1 = ping;
    PingPong s2 = pong;
    std::thread t1(process, s1);
    std::thread t2(process, s2);

    t1.join();
    t2.join();
    return 0;
}
