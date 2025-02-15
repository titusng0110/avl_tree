#include <set>
#include <random>
#include <chrono>
#include "avl_tree.hpp"
#include <iomanip>

// Utility function to generate random integers
std::vector<int> generate_random_integers(size_t count)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-100000, 100000);

    std::vector<int> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        result.push_back(dis(gen));
    }
    return result;
}

// Timer class for measuring execution time
class Timer
{
private:
    std::chrono::high_resolution_clock::time_point start_time;

public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

    double elapsed()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end_time - start_time).count();
    }
};

// Function to run performance tests
void run_performance_test(size_t data_size)
{
    std::cout << "\nTesting with data size: " << data_size << std::endl;
    std::cout << std::setw(25) << "Operation" << std::setw(20) << "AVLTree (ms)"
              << std::setw(25) << "std::multiset (ms)" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    const auto data = generate_random_integers(data_size);

    // Test initialize (10 times)
    {
        double avl_time = 0, ms_time = 0;
        
        for (int i = 0; i < 10; ++i)
        {
            AVLTree<int> avl;
            Timer t1;
            avl.bulk_insert(data.begin(), data.end());
            avl_time += t1.elapsed();
            avl.clear();
        }
        for(int i = 0; i < 10; ++i)
        {
            Timer t2;
            std::multiset<int> ms(data.begin(), data.end());
            ms_time += t2.elapsed();
            ms.clear();
        }
        std::cout << std::setw(25) << "initialize 10 ops"
                  << std::setw(20) << avl_time
                  << std::setw(25) << ms_time << std::endl;
    }

    // Test insert (50000 operations)
    {
        const auto test_data = generate_random_integers(50000);

        AVLTree<int> avl;
        avl.bulk_insert(data.begin(), data.end());
        Timer t1;
        for (int val : test_data)
            avl.insert(val);
        double avl_time = t1.elapsed();
        avl.clear();

        std::multiset<int> ms(data.begin(), data.end());
        Timer t2;
        for (int val : test_data)
            ms.insert(val);
        double ms_time = t2.elapsed();
        ms.clear();

        std::cout << std::setw(25) << "insert 50000 ops"
                  << std::setw(20) << avl_time
                  << std::setw(25) << ms_time << std::endl;
    }

    // Test insert_multiple (50000 operations)
    {
        const auto test_data = generate_random_integers(50000);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dup_dist(2, 100);

        // Generate duplicate counts first
        std::vector<int> duplicate_counts;
        duplicate_counts.reserve(test_data.size());
        for (size_t i = 0; i < test_data.size(); ++i)
        {
            duplicate_counts.push_back(dup_dist(gen));
        }
        
        AVLTree<int> avl;
        avl.bulk_insert(data.begin(), data.end());
        Timer t1;
        for (size_t i = 0; i < test_data.size(); ++i)
        {
            avl.insert_multiple(test_data[i], duplicate_counts[i]);
        }
        double avl_time = t1.elapsed();
        avl.clear();

        std::multiset<int> ms(data.begin(), data.end());
        Timer t2;
        for (size_t i = 0; i < test_data.size(); ++i)
        {
            for (int j = 0; j < duplicate_counts[i]; ++j)
            {
                ms.insert(test_data[i]);
            }
        }
        double ms_time = t2.elapsed();
        ms.clear();

        std::cout << std::setw(25) << "insert_multiple 50000 ops"
                  << std::setw(20) << avl_time
                  << std::setw(25) << ms_time << std::endl;
    }

    // Test bulk_insert (10 times)
    {
        double avl_time = 0, ms_time = 0;
        for (int i = 0; i < 10; ++i)
        {
            const auto test_data = generate_random_integers(data_size);

            AVLTree<int> avl;
            avl.bulk_insert(data.begin(), data.end());
            Timer t1;
            avl.bulk_insert(test_data.begin(), test_data.end());
            avl_time += t1.elapsed();
            avl.clear();

            std::multiset<int> ms(data.begin(), data.end());
            Timer t2;
            ms.insert(test_data.begin(), test_data.end());
            ms_time += t2.elapsed();
            ms.clear();
        }

        std::cout << std::setw(25) << "bulk_insert 10 ops"
                  << std::setw(20) << avl_time
                  << std::setw(25) << ms_time << std::endl;
    }

    // Test remove (50000 operations)
    {
        const auto test_data = generate_random_integers(50000);

        AVLTree<int> avl;
        avl.bulk_insert(data.begin(), data.end());
        Timer t1;
        for (int val : test_data)
            avl.remove(val);
        double avl_time = t1.elapsed();
        avl.clear();

        std::multiset<int> ms(data.begin(), data.end());
        Timer t2;
        for (int val : test_data)
        {
            auto it = ms.find(val);
            if (it != ms.end())
                ms.erase(it);
        }
        double ms_time = t2.elapsed();
        ms.clear();

        std::cout << std::setw(25) << "remove 50000 ops"
                  << std::setw(20) << avl_time
                  << std::setw(25) << ms_time << std::endl;
    }

    // Test remove_multiple (50000 operations)
    {
        const auto test_data = generate_random_integers(50000);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dup_dist(2, 100); // Random duplicates between 2-100

        // Generate duplicate counts first
        std::vector<int> duplicate_counts;
        duplicate_counts.reserve(test_data.size());
        for (size_t i = 0; i < test_data.size(); ++i)
        {
            duplicate_counts.push_back(dup_dist(gen));
        }
        
        AVLTree<int> avl;
        avl.bulk_insert(data.begin(), data.end());
        Timer t1;
        for (size_t i = 0; i < test_data.size(); ++i)
        {
            avl.remove_multiple(test_data[i], duplicate_counts[i]);
        }
        double avl_time = t1.elapsed();
        avl.clear();

        std::multiset<int> ms(data.begin(), data.end());
        Timer t2;
        for (size_t i = 0; i < test_data.size(); ++i)
        {
            auto range = ms.equal_range(test_data[i]);
            for (int j = 0; j < duplicate_counts[i] && range.first != range.second; ++j)
            {
                range.first = ms.erase(range.first);
            }
        }
        double ms_time = t2.elapsed();
        ms.clear();

        std::cout << std::setw(25) << "remove_multiple 50000 ops"
                  << std::setw(20) << avl_time
                  << std::setw(25) << ms_time << std::endl;
    }

    // Test min/max operations (50000 operations)
    {
        AVLTree<int> avl;
        avl.bulk_insert(data.begin(), data.end());
        Timer t1;
        for (int i = 0; i < 50000; ++i)
            avl.min();
        double avl_min_time = t1.elapsed();
        avl.clear();
        
        std::multiset<int> ms(data.begin(), data.end());
        Timer t2;
        for (int i = 0; i < 50000; ++i)
            *(ms.begin());
        double ms_min_time = t2.elapsed();
        ms.clear();

        std::cout << std::setw(25) << "min 50000 ops"
                  << std::setw(20) << avl_min_time
                  << std::setw(25) << ms_min_time << std::endl;

        avl.bulk_insert(data.begin(), data.end());
        Timer t3;
        for (int i = 0; i < 50000; ++i)
            avl.max();
        double avl_max_time = t3.elapsed();
        avl.clear();

        ms.insert(data.begin(), data.end());
        Timer t4;
        for (int i = 0; i < 50000; ++i)
            *(--ms.end());
        double ms_max_time = t4.elapsed();
        ms.clear();

        std::cout << std::setw(25) << "max 50000 ops"
                  << std::setw(20) << avl_max_time
                  << std::setw(25) << ms_max_time << std::endl;
    }

    // Test pop operations (50000 operations)
    {
        AVLTree<int> avl;
        avl.bulk_insert(data.begin(), data.end());
        Timer t1;
        for (int i = 0; i < 50000 && avl.size() > 0; ++i)
            avl.pop_min();
        double avl_min_time = t1.elapsed();
        avl.clear();

        std::multiset<int> ms(data.begin(), data.end());
        Timer t2;
        for (int i = 0; i < 50000 && !ms.empty(); ++i)
            ms.erase(ms.begin());
        double ms_min_time = t2.elapsed();
        ms.clear();

        std::cout << std::setw(25) << "pop_min 50000 ops"
                  << std::setw(20) << avl_min_time
                  << std::setw(25) << ms_min_time << std::endl;

        avl.bulk_insert(data.begin(), data.end());
        Timer t3;
        for (int i = 0; i < 50000 && avl.size() > 0; ++i)
            avl.pop_max();
        double avl_max_time = t3.elapsed();
        avl.clear();

        ms.insert(data.begin(), data.end());
        Timer t4;
        for (int i = 0; i < 50000 && !ms.empty(); ++i)
            ms.erase(--ms.end());
        double ms_max_time = t4.elapsed();
        ms.clear();

        std::cout << std::setw(25) << "pop_max 50000 ops"
                  << std::setw(20) << avl_max_time
                  << std::setw(25) << ms_max_time << std::endl;
    }

    // Test lower_bound operations (50000 operations)
    {
        const auto test_data = generate_random_integers(50000);

        AVLTree<int> avl;
        avl.bulk_insert(data.begin(), data.end());
        Timer t1;
        for (int val : test_data)
        {
            auto it = avl.lower_bound(val);
        }
        double avl_time = t1.elapsed();
        avl.clear();

        std::multiset<int> ms(data.begin(), data.end());
        Timer t2;
        for (int val : test_data)
        {
            auto it = ms.lower_bound(val);
        }
        double ms_time = t2.elapsed();
        ms.clear();

        std::cout << std::setw(25) << "lower_bound 50000 ops"
                  << std::setw(20) << avl_time
                  << std::setw(25) << ms_time << std::endl;
    }

    // Test count operations (50000 operations)
    {
        const auto test_data = generate_random_integers(50000);

        AVLTree<int> avl;
        avl.bulk_insert(data.begin(), data.end());
        Timer t1;
        for (int val : test_data)
        {
            avl.count(val);
        }
        double avl_time = t1.elapsed();
        avl.clear();

        std::multiset<int> ms(data.begin(), data.end());
        Timer t2;
        for (int val : test_data)
        {
            ms.count(val);
        }
        double ms_time = t2.elapsed();
        ms.clear();

        std::cout << std::setw(25) << "count 50000 ops"
                  << std::setw(20) << avl_time
                  << std::setw(25) << ms_time << std::endl;
    }
}

int main()
{
    // Test with different data sizes
    run_performance_test(100000);
    run_performance_test(1000000);
    run_performance_test(10000000);

    return 0;
}