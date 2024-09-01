#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

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
    std::vector<int> thread_counts = { 2, 4, 8, 16 };

    std::cout << "Доступное количество аппаратных ядер: " << std::thread::hardware_concurrency() << std::endl;

    for (int size : sizes) {
        std::cout << "\nРазмер векторов: " << size << std::endl;

        std::vector<int> a(size, 1);
        std::vector<int> b(size, 2);
        std::vector<int> result(size);

        for (int num_threads : thread_counts) {
            double time_taken = measure_time(num_threads, a, b, result);
            std::cout << "Потоков: " << num_threads << ", Время: " << time_taken << " сек" << std::endl;
        }
    }

    return 0;
}
