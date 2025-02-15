#include "avl_tree.hpp"
#include <cassert>
#include <vector>
#include <list>
#include <iostream>
#include <string>

// Helper function to verify if a vector is sorted
template <typename T>
bool is_sorted(const std::vector<T> &vec)
{
    return std::is_sorted(vec.begin(), vec.end());
}

// Helper function to convert tree to vector for verification
template <typename T>
std::vector<T> tree_to_vector(const AVLTree<T> &tree)
{
    std::vector<T> result;
    tree.print_inorder(); // This will help in debugging
    // We'll use the fact that print_inorder already gives us elements in order
    return result;
}

void test_edge_cases()
{
    std::cout << "\nTesting edge cases..." << std::endl;

    // Test Case 1: Empty bulk insert into empty tree
    {
        AVLTree<int> tree;
        std::vector<int> empty;
        tree.bulk_insert(empty.begin(), empty.end());
        assert(tree.size() == 0);
        std::cout << "Edge case 1 passed: Empty bulk insert into empty tree" << std::endl;
    }

    // Test Case 2: Empty bulk insert into non-empty tree
    {
        AVLTree<int> tree;
        tree.insert(1);
        std::vector<int> empty;
        tree.bulk_insert(empty.begin(), empty.end());
        assert(tree.size() == 1);
        assert(tree.contains(1));
        std::cout << "Edge case 2 passed: Empty bulk insert into non-empty tree" << std::endl;
    }

    // Test Case 3: Bulk insert with duplicates
    {
        AVLTree<int> tree;
        std::vector<int> data = {1, 1, 1, 1};
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 4);
        assert(tree.count(1) == 4);
        std::cout << "Edge case 3 passed: Bulk insert with duplicates" << std::endl;
    }

    // Test Case 4: Bulk insert with different container type (list)
    {
        AVLTree<int> tree;
        std::list<int> data = {1, 2, 3};
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 3);
        std::cout << "Edge case 4 passed: Bulk insert with different container type" << std::endl;
    }

    // Test Case 5: Bulk insert into tree with existing elements
    {
        AVLTree<int> tree;
        tree.insert(5);
        std::vector<int> data = {1, 2, 3};
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 4);
        assert(tree.contains(5));
        std::cout << "Edge case 5 passed: Bulk insert into tree with existing elements" << std::endl;
    }
}

void test_normal_cases()
{
    std::cout << "\nTesting normal cases..." << std::endl;

    // Test Case 1: Simple ascending sequence
    {
        AVLTree<int> tree;
        std::vector<int> data = {1, 2, 3, 4, 5};
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 5);
        assert(tree.min() == 1);
        assert(tree.max() == 5);
        std::cout << "Normal case 1 passed: Simple ascending sequence" << std::endl;
    }

    // Test Case 2: Simple descending sequence
    {
        AVLTree<int> tree;
        std::vector<int> data = {5, 4, 3, 2, 1};
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 5);
        assert(tree.min() == 1);
        assert(tree.max() == 5);
        std::cout << "Normal case 2 passed: Simple descending sequence" << std::endl;
    }

    // Test Case 3: Random sequence
    {
        AVLTree<int> tree;
        std::vector<int> data = {8, 3, 10, 1, 6, 14, 4, 7, 13};
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 9);
        assert(tree.min() == 1);
        assert(tree.max() == 14);
        std::cout << "Normal case 3 passed: Random sequence" << std::endl;
    }

    // Test Case 4: Sequence with negative numbers
    {
        AVLTree<int> tree;
        std::vector<int> data = {-5, -3, -1, 0, 2, 4};
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 6);
        assert(tree.min() == -5);
        assert(tree.max() == 4);
        std::cout << "Normal case 4 passed: Sequence with negative numbers" << std::endl;
    }

    // Test Case 5: Multiple bulk inserts
    {
        AVLTree<int> tree;
        std::vector<int> data1 = {1, 3, 5};
        std::vector<int> data2 = {2, 4, 6};
        tree.bulk_insert(data1.begin(), data1.end());
        tree.bulk_insert(data2.begin(), data2.end());
        assert(tree.size() == 6);
        assert(tree.min() == 1);
        assert(tree.max() == 6);
        std::cout << "Normal case 5 passed: Multiple bulk inserts" << std::endl;
    }

    // Test Case 6: Large number of elements
    {
        AVLTree<int> tree;
        std::vector<int> data(1000);
        for (int i = 0; i < 1000; i++)
            data[i] = i;
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 1000);
        assert(tree.min() == 0);
        assert(tree.max() == 999);
        std::cout << "Normal case 6 passed: Large number of elements" << std::endl;
    }

    // Test Case 7: Mixed positive and negative with duplicates
    {
        AVLTree<int> tree;
        std::vector<int> data = {-2, -2, 0, 0, 3, 3};
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 6);
        assert(tree.count(-2) == 2);
        assert(tree.count(0) == 2);
        assert(tree.count(3) == 2);
        std::cout << "Normal case 7 passed: Mixed positive and negative with duplicates" << std::endl;
    }

    // Test Case 8: Single element repeated many times
    {
        AVLTree<int> tree;
        std::vector<int> data(100, 42);
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 100);
        assert(tree.count(42) == 100);
        std::cout << "Normal case 8 passed: Single element repeated many times" << std::endl;
    }

    // Test Case 9: String type test
    {
        AVLTree<std::string> tree;
        std::vector<std::string> data = {"apple", "banana", "cherry", "date"};
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 4);
        assert(tree.min() == "apple");
        assert(tree.max() == "date");
        std::cout << "Normal case 9 passed: String type test" << std::endl;
    }

    // Test Case 10: Floating point numbers
    {
        AVLTree<double> tree;
        std::vector<double> data = {1.1, 2.2, 3.3, 4.4, 5.5};
        tree.bulk_insert(data.begin(), data.end());
        assert(tree.size() == 5);
        assert(tree.min() == 1.1);
        assert(tree.max() == 5.5);
        std::cout << "Normal case 10 passed: Floating point numbers" << std::endl;
    }
}

void test_tree_larger_than_bulk()
{
    std::cout << "\nTesting cases where tree size > bulk size..." << std::endl;

    // Test Case 1: Large tree, small bulk insert
    {
        AVLTree<int> tree;
        // First create a large tree
        for (int i = 0; i < 1000; i++)
        {
            tree.insert(i * 2); // Even numbers
        }
        int original_size = tree.size();

        // Small bulk insert
        std::vector<int> small_bulk = {1, 3, 5, 7, 9}; // Odd numbers
        tree.bulk_insert(small_bulk.begin(), small_bulk.end());

        assert(tree.size() == original_size + small_bulk.size());
        assert(tree.contains(1));
        assert(tree.contains(9));
        std::cout << "Large tree small bulk case 1 passed" << std::endl;
    }

    // Test Case 2: Large tree, bulk insert with duplicates
    {
        AVLTree<int> tree;
        // Create large tree
        for (int i = 0; i < 500; i++)
        {
            tree.insert(i);
        }
        int original_size = tree.size();

        // Bulk insert with some duplicates
        std::vector<int> bulk = {0, 1, 2, 499, 500}; // Some existing, one new
        tree.bulk_insert(bulk.begin(), bulk.end());

        assert(tree.size() == original_size + bulk.size());
        assert(tree.contains(500));
        assert(tree.count(0) == 2);
        std::cout << "Large tree small bulk case 2 passed" << std::endl;
    }

    // Test Case 3: Large tree, bulk insert in different ranges
    {
        AVLTree<int> tree;
        // Create large tree with numbers 1000-2000
        for (int i = 1000; i <= 2000; i++)
        {
            tree.insert(i);
        }
        int original_size = tree.size();

        // Bulk insert numbers below, within, and above the range
        std::vector<int> bulk = {500, 1500, 2500};
        tree.bulk_insert(bulk.begin(), bulk.end());

        assert(tree.size() == original_size + bulk.size());
        assert(tree.contains(500));
        assert(tree.contains(2500));
        std::cout << "Large tree small bulk case 3 passed" << std::endl;
    }

    // Test Case 4: Large tree with bulk insert at boundaries
    {
        AVLTree<int> tree;
        // Create large tree
        for (int i = 100; i < 1000; i++)
        {
            tree.insert(i);
        }
        int original_size = tree.size();

        // Bulk insert at boundaries
        std::vector<int> bulk = {99, 1000}; // Just outside current min/max
        tree.bulk_insert(bulk.begin(), bulk.end());

        assert(tree.size() == original_size + 2);
        assert(tree.min() == 99);
        assert(tree.max() == 1000);
        std::cout << "Large tree small bulk case 4 passed" << std::endl;
    }

    // Test Case 5: Large tree with scattered bulk insert
    {
        AVLTree<int> tree;
        // Create large tree with even numbers
        for (int i = 0; i < 1000; i += 2)
        {
            tree.insert(i);
        }
        int original_size = tree.size();

        // Bulk insert some odd numbers scattered throughout
        std::vector<int> bulk = {1, 101, 501, 901};
        tree.bulk_insert(bulk.begin(), bulk.end());

        assert(tree.size() == original_size + bulk.size());
        for (int x : bulk)
        {
            assert(tree.contains(x));
        }
        std::cout << "Large tree small bulk case 5 passed" << std::endl;
    }
}

int main()
{
    std::cout << "Starting bulk_insert tests..." << std::endl;

    test_edge_cases();
    test_normal_cases();
    test_tree_larger_than_bulk();

    std::cout << "\nAll tests passed successfully!" << std::endl;
    return 0;
}