#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool evenTurn = true; // Start with even number (0)

void printEven() {
    for (int i = 0; i <= 10; i += 2) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return evenTurn; });

        std::cout << "Even: " << i << std::endl;

        evenTurn = false;
        cv.notify_all(); // Notify odd thread
    }
}

void printOdd() {
    for (int i = 1; i < 10; i += 2) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !evenTurn; });

        std::cout << "Odd : " << i << std::endl;

        evenTurn = true;
        cv.notify_all(); // Notify even thread
    }
}

int main() {
    std::thread t1(printEven);
    std::thread t2(printOdd);

    t1.join();
    t2.join();

    return 0;
}
