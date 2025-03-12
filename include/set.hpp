#ifndef SET_HPP
#define SET_HPP

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace AVLTree {

    template <typename T>
    class Set
    {
    private:
        struct Node
        {
            T key;
            short height;
            Node *left;
            Node *right;
            Node(const T &k, size_t cnt = 1)
                : key(k), height(1), count(cnt), left(nullptr), right(nullptr) {}
        };

        Node *root;
        Node *min_node;
        Node *max_node;
        size_t distinct_count;

        short height(Node *node) const;
        Node *buildFromSorted(const std::vector<T> &keys, size_t start, size_t end);
        int getBalance(Node *node) const;
        Node *rotateRight(Node *y);
        Node *rotateLeft(Node *x);
        Node *insert(Node *node, const T &key, size_t amount);
        Node *getMinNode(Node *node) const;
        Node *getMaxNode(Node *node) const;
        void updateMinNode();
        void updateMaxNode();
        Node *remove(Node *node, const T &key, size_t amount);
        void clear(Node *node);
        Node *lower_bound(Node *node, const T &key) const;
        void inorder(Node *node, std::vector<T> &result) const;

    public:
        Set();
        template <typename Iterator>
        Set(Iterator begin, Iterator end);
        ~Set();
        template <typename Iterator>
        void insert(Iterator begin, Iterator end);
        void insert(const T &key);
        void remove(const T &key);
        size_t count(const T &key) const;
        bool contains(const T &key) const;
        T min() const;
        T max() const;
        T pop_min();
        T pop_max();
        size_t size() const;
        bool empty() const;
        size_t distinct_size() const;
        void clear();
        std::vector<T> to_vector() const;
    };

} // namespace AVLTree

#endif // SET_HPP