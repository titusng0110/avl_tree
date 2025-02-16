#include <set>
#include <random>
#include <chrono>
#include <iomanip>
#include <iostream>
#include "avl_tree.hpp"

class Timer
{
private:
    std::chrono::high_resolution_clock::time_point start;

public:
    Timer() : start(std::chrono::high_resolution_clock::now()) {}

    double elapsed() const
    {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};

std::vector<int> generate_random_data(size_t count)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-10000000, 10000000);

    std::vector<int> data;
    data.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        data.push_back(dist(gen));
    }
    return data;
}

void print_result(const std::string &operation, double avl_time, double std_time)
{
    std::cout << std::left << std::setw(30) << operation
              << std::setw(15) << avl_time
              << std::setw(15) << std_time << std::endl;
}

void benchmark_operations(size_t data_size)
{
    std::cout << "\nBenchmarking with size: " << data_size << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::left
              << std::setw(30) << "Operation"
              << std::setw(15) << "AVLTree (ms)"
              << std::setw(15) << "std::multiset (ms)" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // Generate test data
    auto initial_data = generate_random_data(data_size);
    auto test_data = generate_random_data(50000);

    double avl_time, std_time;

    // Test initialization
    {
        Timer t1;
        AVLTree<int> avl(initial_data.begin(), initial_data.end());
        avl_time = t1.elapsed();
        avl.clear();
    }
    {
        Timer t2;
        std::multiset<int> ms(initial_data.begin(), initial_data.end());
        std_time = t2.elapsed();
        ms.clear();
    }
    print_result("Initialize", avl_time, std_time);

    // Test insertions
    {
        AVLTree<int> avl;
        Timer t1;
        for (int val : test_data)
        {
            avl.insert(val);
        }
        avl_time = t1.elapsed();
        avl.clear();
    }
    {
        std::multiset<int> ms;
        Timer t2;
        for (int val : test_data)
        {
            ms.insert(val);
        }
        std_time = t2.elapsed();
        ms.clear();
    }
    print_result("Insert (50K ops)", avl_time, std_time);

    // Test multiple insertions
    {
        AVLTree<int> avl;
        Timer t1;
        for (int val : test_data)
        {
            avl.insert_multiple(val, 5);
        }
        avl_time = t1.elapsed();
        avl.clear();
    }
    {
        std::multiset<int> ms;
        Timer t2;
        for (int val : test_data)
        {
            for (int i = 0; i < 5; ++i)
            {
                ms.insert(val);
            }
        }
        std_time = t2.elapsed();
        ms.clear();
    }
    print_result("Insert Multiple (50K×5)", avl_time, std_time);

    // Test search operations
    {
        AVLTree<int> avl(initial_data.begin(), initial_data.end());
        Timer t1;
        for (int val : test_data)
        {
            avl.contains(val);
        }
        avl_time = t1.elapsed();
        avl.clear();
    }
    {
        std::multiset<int> ms(initial_data.begin(), initial_data.end());
        Timer t2;
        for (int val : test_data)
        {
            ms.count(val);
        }
        std_time = t2.elapsed();
        ms.clear();
    }
    print_result("Search (50K ops)", avl_time, std_time);

    // Test count operations
    {
        AVLTree<int> avl(initial_data.begin(), initial_data.end());
        Timer t1;
        for (int val : test_data)
        {
            avl.count(val);
        }
        avl_time = t1.elapsed();
        avl.clear();
    }
    {
        std::multiset<int> ms(initial_data.begin(), initial_data.end());
        Timer t2;
        for (int val : test_data)
        {
            ms.count(val);
        }
        std_time = t2.elapsed();
        ms.clear();
    }
    print_result("Count (50K ops)", avl_time, std_time);

    // Test min/max operations
    {
        AVLTree<int> avl(initial_data.begin(), initial_data.end());
        Timer t1;
        for (int i = 0; i < 25000; ++i)
        {
            avl.min();
            avl.max();
        }
        avl_time = t1.elapsed();
        avl.clear();
    }
    {
        std::multiset<int> ms(initial_data.begin(), initial_data.end());
        Timer t2;
        for (int i = 0; i < 25000; ++i)
        {
            auto min = ms.begin();
            auto max = --ms.end();
        }
        std_time = t2.elapsed();
        ms.clear();
    }
    print_result("Min/Max (25K ops each)", avl_time, std_time);

    // Test removals
    {
        AVLTree<int> avl(initial_data.begin(), initial_data.end());
        Timer t1;
        for (int val : test_data)
        {
            avl.remove(val);
        }
        avl_time = t1.elapsed();
        avl.clear();
    }
    {
        std::multiset<int> ms(initial_data.begin(), initial_data.end());
        Timer t2;
        for (int val : test_data)
        {
            auto it = ms.find(val);
            if (it != ms.end())
            {
                ms.erase(it);
            }
        }
        std_time = t2.elapsed();
        ms.clear();
    }
    print_result("Remove (50K ops)", avl_time, std_time);
}

int main()
{
    benchmark_operations(10000);
    benchmark_operations(100000);
    benchmark_operations(1000000);
    benchmark_operations(10000000);
    return 0;
}