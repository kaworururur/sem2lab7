#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <atomic>

std::atomic<int> flag{ 0 };

void threadFunction(std::vector<double>& arr, const std::string& name) {
    int n = arr.size();
    int start = 0, end = n;
    if (name == "thread1") {
        end = n / 2;
    }
    else if (name == "thread2") {
        start = n / 2;
    }

    std::sort(arr.begin() + start, arr.begin() + end);

    for (double val : arr) {
        std::cout << name << ": " << val << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    flag++;
}

int main() {
    const int arraySize = 10;
    std::vector<double> arr(arraySize);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(1.0, 100.0);

    for (int i = 0; i < arraySize; ++i) {
        arr[i] = dis(gen);
    }

    std::thread t1(threadFunction, std::ref(arr), "thread1");
    std::thread t2(threadFunction, std::ref(arr), "thread2");

    while (flag.load() < 2) {
        // Ждем завершения обоих потоков
    }

    std::sort(arr.begin(), arr.end());

    std::cout << "Main thread: Sorting full array...\n";
    for (double val : arr) {
        std::cout << "thread3: " << val << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    t1.join();
    t2.join();

    std::cout << "Main thread: All threads have finished.\n";

    return 0;
}
