#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <mutex>
#include <condition_variable>

// Глобальные переменные
std::mutex mtx;
std::condition_variable cv;
bool thread1Finished = false;
bool thread2Finished = false;

// Потоковая функция для сортировки заданной части массива
void sortArrayPart(const std::vector<double>& arr, int start, int end, const std::string& threadName) {
    std::vector<double> subArr(arr.begin() + start, arr.begin() + end);
    std::sort(subArr.begin(), subArr.end());

    for (double val : subArr) {
        std::mutex mtx;
        mtx.lock();
        std::cout << threadName << ": " << val << std::endl;
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (threadName == "thread1") {
        thread1Finished = true;
    }
    else if (threadName == "thread2") {
        thread2Finished = true;
    }

    cv.notify_all();
}

int main() {
    const int arraySize = 10;
    std::vector<double> arr(arraySize);

    // Заполнение массива случайными числами
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(1.0, 100.0);

    for (int i = 0; i < arraySize; ++i) {
        arr[i] = dis(gen);
    }

    std::thread t1(sortArrayPart, arr, 0, arraySize / 2, "thread1");
    std::thread t2(sortArrayPart, arr, arraySize / 2, arraySize, "thread2");

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return thread1Finished && thread2Finished; });
    }

    // Сортировка всего массива после завершения первых двух потоков
    std::sort(arr.begin(), arr.end());
    std::cout << "thread3: sorted array:\n";
    for (double val : arr) {
        std::cout << "thread3: " << val << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    t1.detach();
    t2.detach();

    std::cout << "Main thread: All threads have finished.\n";

    return 0;
}
