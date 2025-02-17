#ifndef AVL_TREE_BASE_HPP
#define AVL_TREE_BASE_HPP

#include <algorithm>
#include <stdexcept>

namespace AVLTree {

template <typename T>
class AVLTreeBase {
protected:
    struct BaseNode {
        T key;
        short height;
        size_t subtree_size; // not yet implemented
        BaseNode* left;
        BaseNode* right;
        
        BaseNode(const T& k);
        virtual ~BaseNode() = default;
    };

    BaseNode* root;
    BaseNode* min_node;
    BaseNode* max_node;
    size_t distinct_size;

    // Method declarations
    short height(BaseNode* node) const;
    int getBalance(BaseNode* node) const;
    BaseNode* rotateRight(BaseNode* y);
    BaseNode* rotateLeft(BaseNode* x);
    BaseNode* getMinNode(BaseNode* node) const;
    BaseNode* getMaxNode(BaseNode* node) const;
    void updateMinNode();
    void updateMaxNode();
    BaseNode* lower_bound(BaseNode* node, const T& key) const;
    virtual void clearNode(BaseNode* node);
    BaseNode* rebalance(BaseNode* node);

    // Constructor and destructor
    AVLTreeBase();
    virtual ~AVLTreeBase();

    // Prevent copying
    AVLTreeBase(const AVLTreeBase&) = delete;
    AVLTreeBase& operator=(const AVLTreeBase&) = delete;

public:
    bool empty() const;
    T min() const;
    T max() const;
    bool contains(const T& key) const;
    void clear();
};


// Constructor implementations
template <typename T>
AVLTreeBase<T>::BaseNode::BaseNode(const T& k)
    : key(k), height(1), subtree_size(1), left(nullptr), right(nullptr) {}

template <typename T>
AVLTreeBase<T>::AVLTreeBase() 
    : root(nullptr), min_node(nullptr), max_node(nullptr), distinct_size(0) {}

template <typename T>
AVLTreeBase<T>::~AVLTreeBase() {
    clearNode(root);
}

// Method implementations
template <typename T>
short AVLTreeBase<T>::height(BaseNode* node) const {
    return (node == nullptr) ? 0 : node->height;
}

template <typename T>
int AVLTreeBase<T>::getBalance(BaseNode* node) const {
    return (node == nullptr) ? 0 : height(node->left) - height(node->right);
}

template <typename T>
typename AVLTreeBase<T>::BaseNode* AVLTreeBase<T>::rotateRight(BaseNode* y) {
    BaseNode* x = y->left;
    BaseNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + std::max(height(y->left), height(y->right));
    x->height = 1 + std::max(height(x->left), height(x->right));

    return x;
}

template <typename T>
typename AVLTreeBase<T>::BaseNode* AVLTreeBase<T>::rotateLeft(BaseNode* x) {
    BaseNode* y = x->right;
    BaseNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));

    return y;
}

template <typename T>
typename AVLTreeBase<T>::BaseNode* AVLTreeBase<T>::getMinNode(BaseNode* node) const {
    BaseNode* current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

template <typename T>
typename AVLTreeBase<T>::BaseNode* AVLTreeBase<T>::getMaxNode(BaseNode* node) const {
    BaseNode* current = node;
    while (current && current->right != nullptr)
        current = current->right;
    return current;
}

template <typename T>
void AVLTreeBase<T>::updateMinNode() {
    min_node = (root == nullptr) ? nullptr : getMinNode(root);
}

template <typename T>
void AVLTreeBase<T>::updateMaxNode() {
    max_node = (root == nullptr) ? nullptr : getMaxNode(root);
}

template <typename T>
typename AVLTreeBase<T>::BaseNode* AVLTreeBase<T>::lower_bound(BaseNode* node, const T& key) const {
    BaseNode* result = nullptr;
    while (node) {
        if (node->key >= key) {
            result = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return result;
}

template <typename T>
void AVLTreeBase<T>::clearNode(BaseNode* node) {
    if (node == nullptr) return;
    clearNode(node->left);
    clearNode(node->right);
    delete node;
}

template <typename T>
bool AVLTreeBase<T>::empty() const {
    return root == nullptr;
}

template <typename T>
T AVLTreeBase<T>::min() const {
    if (!min_node)
        throw std::runtime_error("Tree is empty");
    return min_node->key;
}

template <typename T>
T AVLTreeBase<T>::max() const {
    if (!max_node)
        throw std::runtime_error("Tree is empty");
    return max_node->key;
}

template <typename T>
bool AVLTreeBase<T>::contains(const T& key) const {
    BaseNode* node = lower_bound(root, key);
    return node != nullptr && node->key == key;
}

template <typename T>
void AVLTreeBase<T>::clear() {
    clearNode(root);
    root = nullptr;
    min_node = nullptr;
    max_node = nullptr;
}

template <typename T>
typename AVLTreeBase<T>::BaseNode* AVLTreeBase<T>::rebalance(BaseNode* node) {
    if (!node) return nullptr;

    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    // Right Right Case
    else if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);

    // Left Right Case
    else if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    else if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

} // namespace AVLTree

#endif // AVL_TREE_BASE_HPP