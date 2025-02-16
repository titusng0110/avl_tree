#include "avl_tree.hpp"
#include <set>
#include <vector>
#include <iostream>
#include <cassert>
#include <random>
#include <algorithm>

// Helper function to print containers
template <typename Container>
void print_container(const Container &c)
{
    for (const auto &element : c)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

void test_avl_tree()
{
    std::cout << "=== Starting AVL Tree Tests ===" << std::endl;

    // Test constructor and bulk insertion
    std::cout << "\nTesting constructors and bulk insertion:" << std::endl;
    std::vector<int> init_data = {5, 3, 7, 2, 4, 6, 8, 3, 5, 7};
    AVLTree<int> avl1(init_data.begin(), init_data.end());
    std::multiset<int> reference(init_data.begin(), init_data.end());

    assert(avl1.size() == reference.size());
    assert(avl1.to_vector() == std::vector<int>(reference.begin(), reference.end()));

    // Test distinct_size
    std::cout << "\nTesting distinct_size:" << std::endl;
    std::set<int> unique_elements(init_data.begin(), init_data.end());
    assert(avl1.distinct_size() == unique_elements.size());
    std::cout << "Distinct size: " << avl1.distinct_size() << std::endl;

    // Test count and contains
    std::cout << "\nTesting count and contains:" << std::endl;
    assert(avl1.count(3) == 2);
    assert(avl1.count(5) == 2);
    assert(avl1.count(9) == 0);
    assert(avl1.contains(3));
    assert(!avl1.contains(9));

    // Test insert_multiple and remove_multiple
    std::cout << "\nTesting insert_multiple and remove_multiple:" << std::endl;
    avl1.insert_multiple(1, 3);
    assert(avl1.count(1) == 3);
    avl1.remove_multiple(1, 2);
    assert(avl1.count(1) == 1);

    // Test remove_all
    std::cout << "\nTesting remove_all:" << std::endl;
    avl1.remove_all(3);
    assert(avl1.count(3) == 0);
    assert(!avl1.contains(3));

    // Test min/max operations
    std::cout << "\nTesting min/max operations:" << std::endl;
    assert(avl1.min() == *avl1.to_vector().begin());
    assert(avl1.max() == avl1.to_vector().back());

    int min_val = avl1.pop_min();
    int max_val = avl1.pop_max();
    std::cout << "Popped min: " << min_val << ", max: " << max_val << std::endl;

    // Test clear and empty
    std::cout << "\nTesting clear and empty:" << std::endl;
    assert(!avl1.empty());
    avl1.clear();
    assert(avl1.empty());
    assert(avl1.size() == 0);

    // Stress test with random operations
    std::cout << "\nPerforming stress test with random operations:" << std::endl;
    AVLTree<int> avl2;
    std::multiset<int> reference2;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100, 100);
    std::uniform_int_distribution<> op_dis(0, 4);

    for (int i = 0; i < 1000; ++i)
    {
        int op = op_dis(gen);
        int val = dis(gen);

        switch (op)
        {
        case 0:
        { // Insert
            avl2.insert(val);
            reference2.insert(val);
            break;
        }
        case 1:
        { // Insert multiple
            int count = std::uniform_int_distribution<>(1, 5)(gen);
            avl2.insert_multiple(val, count);
            for (int j = 0; j < count; ++j)
            {
                reference2.insert(val);
            }
            break;
        }
        case 2:
        { // Remove
            avl2.remove(val);
            auto it = reference2.find(val);
            if (it != reference2.end())
            {
                reference2.erase(it);
            }
            break;
        }
        case 3:
        { // Remove multiple
            int count = std::uniform_int_distribution<>(1, 5)(gen);
            avl2.remove_multiple(val, count);
            for (int j = 0; j < count; ++j)
            {
                auto it = reference2.find(val);
                if (it == reference2.end())
                    break;
                reference2.erase(it);
            }
            break;
        }
        case 4:
        { // Remove all
            avl2.remove_all(val);
            while (true)
            {
                auto it = reference2.find(val);
                if (it == reference2.end())
                    break;
                reference2.erase(it);
            }
            break;
        }
        }

        // Verify consistency
        if (i % 100 == 0)
        {
            assert(avl2.size() == reference2.size());
            assert(avl2.to_vector() == std::vector<int>(reference2.begin(), reference2.end()));
            if (!avl2.empty())
            {
                assert(avl2.min() == *reference2.begin());
                assert(avl2.max() == *std::prev(reference2.end()));
            }
        }
    }

    std::cout << "All tests passed successfully!" << std::endl;
}

int main()
{
    test_avl_tree();
    return 0;
}