#include <set>
#include <random>
#include <chrono>
#include "avl_tree.hpp"
#include <iomanip>

// Utility function to generate random integers
std::vector<int> generate_random_integers(size_t count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-2147483647, 2147483647);
    
    std::vector<int> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        result.push_back(dis(gen));
    }
    return result;
}

// Timer class for measuring execution time
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}
    
    double elapsed() {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end_time - start_time).count();
    }
};

// Function to run performance tests
void run_performance_test(size_t data_size) {
    std::cout << "\nTesting with data size: " << data_size << std::endl;
    std::cout << std::setw(20) << "Operation" << std::setw(15) << "AVLTree (ms)" 
              << std::setw(15) << "std::multiset (ms)" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    // Generate test data
    std::vector<int> data = generate_random_integers(data_size);
    
    // Test init_from_vector (10 times)
    {
        double avl_time = 0, set_time = 0;
        for (int i = 0; i < 10; ++i) {
            AVLTree<int> avl;
            std::multiset<int> set;
            
            Timer t1;
            avl.init_from_vector(data);
            avl_time += t1.elapsed();
            
            Timer t2;
            set.insert(data.begin(), data.end());
            set_time += t2.elapsed();
        }
        std::cout << std::setw(20) << "init_from_vector" 
                  << std::setw(15) << avl_time/10 
                  << std::setw(15) << set_time/10 << std::endl;
    }

    // Initialize containers for other tests
    AVLTree<int> avl;
    std::multiset<int> set;
    avl.init_from_vector(data);
    set.insert(data.begin(), data.end());

    // Test insert (10000 operations)
    {
        auto test_data = generate_random_integers(10000);
        Timer t1;
        for (int val : test_data) avl.insert(val);
        double avl_time = t1.elapsed();
        
        Timer t2;
        for (int val : test_data) set.insert(val);
        double set_time = t2.elapsed();
        
        std::cout << std::setw(20) << "insert" 
                  << std::setw(15) << avl_time 
                  << std::setw(15) << set_time << std::endl;
    }

    // Test insert_multiple (10000 operations)
    {
        auto test_data = generate_random_integers(10000);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dup_dist(2, 20);  // Random duplicates between 2-20
        
        // Generate duplicate counts first
        std::vector<int> duplicate_counts;
        duplicate_counts.reserve(test_data.size());
        for (size_t i = 0; i < test_data.size(); ++i) {
            duplicate_counts.push_back(dup_dist(gen));
        }
        
        Timer t1;
        for (size_t i = 0; i < test_data.size(); ++i) {
            avl.insert_multiple(test_data[i], duplicate_counts[i]);
        }
        double avl_time = t1.elapsed();
        
        Timer t2;
        for (size_t i = 0; i < test_data.size(); ++i) {
            for (int j = 0; j < duplicate_counts[i]; ++j) {
                set.insert(test_data[i]);
            }
        }
        double set_time = t2.elapsed();
        
        std::cout << std::setw(20) << "insert_multiple" 
                << std::setw(15) << avl_time 
                << std::setw(15) << set_time << std::endl;
    }

    // Test remove (10000 operations)
    {
        auto test_data = generate_random_integers(10000);
        Timer t1;
        for (int val : test_data) avl.remove(val);
        double avl_time = t1.elapsed();
        
        Timer t2;
        for (int val : test_data) {
            auto it = set.find(val);
            if (it != set.end()) set.erase(it);
        }
        double set_time = t2.elapsed();
        
        std::cout << std::setw(20) << "remove" 
                  << std::setw(15) << avl_time 
                  << std::setw(15) << set_time << std::endl;
    }

    // Test remove_multiple (10000 operations)
    {
        auto test_data = generate_random_integers(10000);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dup_dist(2, 20);  // Random duplicates between 2-20
        
        // Generate duplicate counts first
        std::vector<int> duplicate_counts;
        duplicate_counts.reserve(test_data.size());
        for (size_t i = 0; i < test_data.size(); ++i) {
            duplicate_counts.push_back(dup_dist(gen));
        }
        
        Timer t1;
        for (size_t i = 0; i < test_data.size(); ++i) {
            avl.remove_multiple(test_data[i], duplicate_counts[i]);
        }
        double avl_time = t1.elapsed();
        
        Timer t2;
        for (size_t i = 0; i < test_data.size(); ++i) {
            auto range = set.equal_range(test_data[i]);
            for (int j = 0; j < duplicate_counts[i] && range.first != range.second; ++j) {
                range.first = set.erase(range.first);
            }
        }
        double set_time = t2.elapsed();
        
        std::cout << std::setw(20) << "remove_multiple" 
                << std::setw(15) << avl_time 
                << std::setw(15) << set_time << std::endl;
    }

    // Test min/max operations (10000 operations)
    {
        Timer t1;
        for (int i = 0; i < 10000; ++i) avl.min();
        double avl_min_time = t1.elapsed();
        
        Timer t2;
        for (int i = 0; i < 10000; ++i) *(set.begin());
        double set_min_time = t2.elapsed();
        
        std::cout << std::setw(20) << "min" 
                  << std::setw(15) << avl_min_time 
                  << std::setw(15) << set_min_time << std::endl;

        Timer t3;
        for (int i = 0; i < 10000; ++i) avl.max();
        double avl_max_time = t3.elapsed();
        
        Timer t4;
        for (int i = 0; i < 10000; ++i) *(--set.end());
        double set_max_time = t4.elapsed();
        
        std::cout << std::setw(20) << "max" 
                  << std::setw(15) << avl_max_time 
                  << std::setw(15) << set_max_time << std::endl;
    }

    // Test pop operations (10000 operations)
    {
        Timer t1;
        for (int i = 0; i < 10000 && avl.size() > 0; ++i) avl.pop_min();
        double avl_time = t1.elapsed();
        
        Timer t2;
        for (int i = 0; i < 10000 && !set.empty(); ++i) set.erase(set.begin());
        double set_time = t2.elapsed();
        
        std::cout << std::setw(20) << "pop_min/max" 
                  << std::setw(15) << avl_time 
                  << std::setw(15) << set_time << std::endl;
    }

    {
        auto test_data = generate_random_integers(10000);
        Timer t1;
        for (int val : test_data) {
            auto it = avl.lower_bound(val);  // Testing lower_bound
        }
        double avl_time = t1.elapsed();
        
        Timer t2;
        for (int val : test_data) {
            auto it = set.lower_bound(val);  // STL set lower_bound
        }
        double set_time = t2.elapsed();
        
        std::cout << std::setw(20) << "lower_bound" 
                  << std::setw(15) << avl_time 
                  << std::setw(15) << set_time << std::endl;
    }
}

int main() {
    // Test with different data sizes
    run_performance_test(100000);
    run_performance_test(1000000);
    run_performance_test(10000000);
    
    return 0;
}