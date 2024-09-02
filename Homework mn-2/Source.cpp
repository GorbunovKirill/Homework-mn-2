#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip> 

void parallel_sum(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, int start, int end) {
    for (int i = start; i < end; ++i) {
        result[i] = a[i] + b[i];
    }
}

double measure_time(int num_threads, const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result) {
    std::vector<std::thread> threads;
    int chunk_size = a.size() / num_threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        int start_index = i * chunk_size;
        int end_index = (i == num_threads - 1) ? a.size() : start_index + chunk_size;

        threads.emplace_back(parallel_sum, std::ref(a), std::ref(b), std::ref(result), start_index, end_index);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    return elapsed.count();
}

int main() {

    setlocale(LC_ALL, "Russian");

    std::vector<int> sizes = { 1000, 10000, 100000, 1000000 };
    std::vector<int> thread_counts = { 1, 2, 4, 8, 16 };

    std::cout << "Количество аппаратных ядер - " << std::thread::hardware_concurrency() << std::endl;


    std::cout << std::setw(15) << " "
        << std::setw(10) << "1000"
        << std::setw(10) << "10000"
        << std::setw(10) << "100000"
        << std::setw(12) << "1000000" << std::endl;

    for (int num_threads : thread_counts) {
        std::cout << std::setw(10) << num_threads << " потоков";

        for (int size : sizes) {
            std::vector<int> a(size, 1);
            std::vector<int> b(size, 2);
            std::vector<int> result(size);

            double time_taken = measure_time(num_threads, a, b, result);
            std::cout << std::setw(10) << std::fixed << std::setprecision(6) << time_taken << "s";
        }

        std::cout << std::endl;
    }

    return 0;
}
