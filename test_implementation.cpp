#include "avl_tree.hpp"
#include <set>
#include <vector>
#include <iostream>
#include <cassert>
#include <random>
#include <algorithm>

// Helper function to compare two containers
template <typename T>
bool containers_equal(const std::vector<T> &avl, const std::vector<T> &std_set)
{
    if (avl.size() != std_set.size())
        return false;
    for (size_t i = 0; i < avl.size(); ++i)
    {
        if (avl[i] != std_set[i])
            return false;
    }
    return true;
}

// Test function
void test_avl_tree()
{
    AVLTree<int> avl;
    std::multiset<int> std_set;

    std::cout << "Testing init_from_vector..." << std::endl;
    std::vector<int> init_vec = {5, 2, 8, 2, 1, 9, 5, 3, 7};
    avl.init_from_vector(init_vec);
    std_set.insert(init_vec.begin(), init_vec.end());
    std::cout << "AVL size: " << avl.size() << ", std::multiset size: " << std_set.size() << std::endl;
    avl.print_inorder();
    for (const auto &element : std_set)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    assert(avl.size() == std_set.size());

    std::cout << "Testing insert..." << std::endl;
    avl.insert(4);
    std_set.insert(4);
    std::cout << "AVL size: " << avl.size() << ", std::multiset size: " << std_set.size() << std::endl;
    avl.print_inorder();
    for (const auto &element : std_set)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    assert(avl.size() == std_set.size());

    std::cout << "Testing insert_multiple..." << std::endl;
    avl.insert_multiple(6, 3);
    for (int i = 0; i < 3; ++i)
        std_set.insert(6);
    std::cout << "AVL size: " << avl.size() << ", std::multiset size: " << std_set.size() << std::endl;
    avl.print_inorder();
    for (const auto &element : std_set)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    assert(avl.size() == std_set.size());

    std::cout << "Testing remove..." << std::endl;
    avl.remove(2);
    auto it_remove = std_set.find(2);
    if (it_remove != std_set.end())
    {
        std_set.erase(it_remove);
    }
    std::cout << "AVL size: " << avl.size() << ", std::multiset size: " << std_set.size() << std::endl;
    avl.print_inorder();
    for (const auto &element : std_set)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    assert(avl.size() == std_set.size());

    std::cout << "Testing remove_multiple..." << std::endl;
    avl.remove_multiple(5, 2);
    for (int i = 0; i < 2; ++i)
    {
        auto it = std_set.find(5);
        if (it != std_set.end())
        {
            std_set.erase(it);
        }
    }
    std::cout << "AVL size: " << avl.size() << ", std::multiset size: " << std_set.size() << std::endl;
    avl.print_inorder();
    for (const auto &element : std_set)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    assert(avl.size() == std_set.size());

    std::cout << "Testing remove_all..." << std::endl;
    avl.remove_all(6);
    while (true)
    {
        auto it = std_set.find(6);
        if (it == std_set.end())
            break;
        std_set.erase(it);
    }
    std::cout << "AVL size: " << avl.size() << ", std::multiset size: " << std_set.size() << std::endl;
    avl.print_inorder();
    for (const auto &element : std_set)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    assert(avl.size() == std_set.size());

    std::cout << "Testing lower_bound..." << std::endl;
    auto avl_lb = avl.lower_bound(3);
    auto std_lb = std_set.lower_bound(3);
    assert(avl_lb != nullptr && *std_lb == avl_lb->key);
    std::cout << "Lower bound of 3: AVL - " << avl_lb->key << ", std::multiset - " << *std_lb << std::endl;

    std::cout << "Testing min..." << std::endl;
    assert(avl.min() == *std_set.begin());
    std::cout << "Minimum: AVL - " << avl.min() << ", std::multiset - " << *std_set.begin() << std::endl;

    std::cout << "Testing max..." << std::endl;
    assert(avl.max() == *std::prev(std_set.end()));
    std::cout << "Maximum: AVL - " << avl.max() << ", std::multiset - " << *std::prev(std_set.end()) << std::endl;

    std::cout << "Testing pop_min..." << std::endl;
    int avl_min = avl.pop_min();
    int std_min = *std_set.begin();
    std_set.erase(std_set.begin());
    assert(avl_min == std_min);
    std::cout << "Popped minimum: AVL - " << avl_min << ", std::multiset - " << std_min << std::endl;
    std::cout << "AVL size: " << avl.size() << ", std::multiset size: " << std_set.size() << std::endl;
    avl.print_inorder();
    for (const auto &element : std_set)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    assert(avl.size() == std_set.size());

    std::cout << "Testing pop_max..." << std::endl;
    int avl_max = avl.pop_max();
    int std_max = *std::prev(std_set.end());
    std_set.erase(std::prev(std_set.end()));
    assert(avl_max == std_max);
    std::cout << "Popped maximum: AVL - " << avl_max << ", std::multiset - " << std_max << std::endl;
    std::cout << "AVL size: " << avl.size() << ", std::multiset size: " << std_set.size() << std::endl;
    avl.print_inorder();
    for (const auto &element : std_set)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    assert(avl.size() == std_set.size());

    std::cout << "Testing print_inorder..." << std::endl;
    avl.print_inorder();
    for (const auto &element : std_set)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    std::cout << "Additional tests with random operations..." << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < 1000; ++i)
    {
        int op = dis(gen) % 5;
        int val = dis(gen);
        int amount = 0;

        std::cout << "Operation " << i << ": ";

        switch (op)
        {
        case 0: // Insert
            std::cout << "Insert " << val << std::endl;
            avl.insert(val);
            std_set.insert(val);
            break;
        case 1: // Insert multiple
            amount = dis(gen) % 5 + 1;
            std::cout << "Insert multiple " << val << " x " << amount << std::endl;
            avl.insert_multiple(val, amount);
            for (int j = 0; j < amount; ++j)
            {
                std_set.insert(val);
            }
            break;
        case 2:
        { // Remove
            std::cout << "Remove " << val << std::endl;
            avl.remove(val);
            auto it = std_set.find(val);
            if (it != std_set.end())
            {
                std_set.erase(it);
            }
            break;
        }
        case 3:
        { // Remove multiple
            amount = dis(gen) % 5 + 1;
            std::cout << "Remove multiple " << val << " x " << amount << std::endl;
            avl.remove_multiple(val, amount);
            for (int j = 0; j < amount; ++j)
            {
                auto it = std_set.find(val);
                if (it == std_set.end())
                    break;
                std_set.erase(it);
            }
            break;
        }
        case 4:
        { // Remove all
            std::cout << "Remove all " << val << std::endl;
            avl.remove_all(val);
            while (true)
            {
                auto it = std_set.find(val);
                if (it == std_set.end())
                    break;
                std_set.erase(it);
            }
            break;
        }
        }

        std::cout << "After operation: AVL size: " << avl.size() << ", std::multiset size: " << std_set.size() << std::endl;
        assert(avl.size() == std_set.size());

        if (!std_set.empty())
        {
            assert(avl.min() == *std_set.begin());
            assert(avl.max() == *std::prev(std_set.end()));
        }

        // Check lower_bound for random values
        val = dis(gen);
        auto avl_lb = avl.lower_bound(val);
        auto std_lb = std_set.lower_bound(val);
        if (avl_lb == nullptr)
        {
            assert(std_lb == std_set.end());
        }
        else
        {
            assert(std_lb != std_set.end() && *std_lb == avl_lb->key);
        }
        std::cout << "Lower bound of " << val << ": AVL - " << (avl_lb ? avl_lb->key : -1)
                  << ", std::multiset - " << (std_lb != std_set.end() ? *std_lb : -1) << std::endl;
    }

    std::cout << "All tests passed successfully!" << std::endl;
}

int main()
{
    test_avl_tree();
    return 0;
}