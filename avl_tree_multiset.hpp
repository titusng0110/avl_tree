#ifndef AVL_TREE_MULTISET_HPP
#define AVL_TREE_MULTISET_HPP

#include "avl_tree_base.hpp"
#include <vector>
#include <utility>

namespace AVLTree {

template <typename T>
class MultiSet : public AVLTreeBase<T> {
private:
    // Extend BaseNode to include count for MultiSet
    struct Node : public AVLTreeBase<T>::BaseNode {
        size_t count;
        
        Node(const T& k, size_t cnt = 1) : AVLTreeBase<T>::BaseNode(k), count(cnt) {}
    };

    size_t total_size;

    // Helper methods specific to MultiSet
    Node* buildFromSorted(const std::vector<T>& keys, size_t start, size_t end);
    Node* insert(Node* node, const T& key, size_t amount);
    Node* remove(Node* node, const T& key, size_t amount);
    void inorder(Node* node, std::vector<T>& result) const;
    void inordermap(Node* node, std::vector<std::pair<T, size_t>>& result) const;

    // Override base class virtual method
    void clearNode(BaseNode* node) override {
        if (node == nullptr) return;
        clearNode(node->left);
        clearNode(node->right);
        delete static_cast<Node*>(node);
    }

public:
    MultiSet() : distinct_count(0), total_count(0) {}

    template <typename Iterator>
    MultiSet(Iterator begin, Iterator end) : distinct_count(0), total_count(0) {
        insert(begin, end);
    }

    // Public interface methods
    template <typename Iterator>
    void insert(Iterator begin, Iterator end);
    void insert(const T& key, size_t amount = 1);
    void remove(const T& key, size_t amount = 1);
    void remove_all(const T& key);
    size_t count(const T& key) const;
    size_t size() const { return total_count; }
    size_t distinct_size() const { return distinct_count; }

    T pop_min();
    T pop_max();
    
    std::vector<T> to_vector() const;
    std::vector<std::pair<T, size_t>> to_vectormap() const;

    // Using declarations to bring in base class methods
    using AVLTreeBase<T>::empty;
    using AVLTreeBase<T>::min;
    using AVLTreeBase<T>::max;
    using AVLTreeBase<T>::contains;
    using AVLTreeBase<T>::clear;
};

} // namespace AVLTree

#endif // AVL_TREE_MULTISET_HPP