#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

// Потоковая функция для сортировки массива
void sortArray(std::vector<double>& arr, const std::string& threadName) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Задержка 10 мс

    std::sort(arr.begin(), arr.end());

    for (double elem : arr) {
        std::cout << threadName << ": " << elem << std::endl;
    }
}

int main() {
    std::vector<double> arr(10);

    // Заполнение массива случайными числами
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(1.0, 100.0);
    for (int i = 0; i < arr.size(); ++i) {
        arr[i] = dis(gen);
    }

    std::thread thread1(sortArray, std::ref(arr), "thread1");
    std::thread thread2(sortArray, std::ref(arr), "thread2");

    thread1.join();
    thread2.join();

    std::thread thread3(sortArray, std::ref(arr), "thread3");
    thread3.join();

    std::cout << "Main thread finished" << std::endl;

    return 0;
}