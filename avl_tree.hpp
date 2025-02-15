#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>

template <typename T>
class AVLTree {
private:
    struct Node {
        T key;
        int height;
        int count;
        Node* left;
        Node* right;
        Node(const T &k, int cnt = 1)
            : key(k), height(1), count(cnt), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int distinct_count;
    int total_count;

    int height(Node* node) const;
    Node* buildFromSorted(const std::vector<T>& keys, int start, int end);
    int getBalance(Node* node) const;
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    Node* insert(Node* node, const T &key, int amount);
    Node* minValueNode(Node* node);
    Node* maxValueNode(Node* node);
    Node* remove(Node* node, const T &key, int amount);
    void clear(Node* node);
    Node* lower_bound(Node* node, const T &key) const;
    void inorder(Node* node) const;

public:
    AVLTree();
    ~AVLTree();
    void init_from_vector(const std::vector<T>& keys);
    void insert(const T &key);
    void insert_multiple(const T &key, int amount);
    void remove_multiple(const T &key, int amount);
    void remove_all(const T &key);
    void remove(const T &key);
    Node* lower_bound(const T &key) const;
    T min() const;
    T max() const;
    T pop_min();
    T pop_max();
    int size() const;
    void clear();
    void print_inorder() const;
};

// Constructor and Destructor
template <typename T>
AVLTree<T>::AVLTree() : root(nullptr), distinct_count(0), total_count(0) {}

template <typename T>
AVLTree<T>::~AVLTree() {
    clear(root);
}

// Private Helper Methods
template <typename T>
int AVLTree<T>::height(Node* node) const {
    return (node == nullptr) ? 0 : node->height;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::buildFromSorted(const std::vector<T>& keys, int start, int end) {
    if (start > end)
        return nullptr;

    int mid = (start + end) / 2;
    
    int count = 1;
    int i = mid - 1;
    while (i >= start && keys[i] == keys[mid]) {
        count++;
        i--;
    }
    i = mid + 1;
    while (i <= end && keys[i] == keys[mid]) {
        count++;
        i++;
    }

    Node* node = new Node(keys[mid], count);
    distinct_count++;
    total_count += count;

    int leftEnd = mid - 1;
    while (leftEnd >= start && keys[leftEnd] == keys[mid]) leftEnd--;
    
    int rightStart = mid + 1;
    while (rightStart <= end && keys[rightStart] == keys[mid]) rightStart++;

    node->left = buildFromSorted(keys, start, leftEnd);
    node->right = buildFromSorted(keys, rightStart, end);

    node->height = 1 + std::max(height(node->left), height(node->right));
    
    return node;
}

template <typename T>
int AVLTree<T>::getBalance(Node* node) const {
    return (node == nullptr) ? 0 : height(node->left) - height(node->right);
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    return x;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    return y;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::insert(Node* node, const T &key, int amount) {
    if (node == nullptr) {
        distinct_count++;
        total_count += amount;
        return new Node(key, amount);
    }
    if (key < node->key)
        node->left = insert(node->left, key, amount);
    else if (key > node->key)
        node->right = insert(node->right, key, amount);
    else {
        node->count += amount;
        total_count += amount;
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rotateRight(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::maxValueNode(Node* node) {
    Node* current = node;
    while (current->right != nullptr)
        current = current->right;
    return current;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::remove(Node* node, const T &key, int amount) {
    if (node == nullptr)
        return node;

    if (key < node->key)
        node->left = remove(node->left, key, amount);
    else if (key > node->key)
        node->right = remove(node->right, key, amount);
    else {
        // Found the node with key.
        if (amount < node->count) {
            // Just remove the given amount.
            node->count -= amount;
            total_count -= amount;
            return node;
        } else {
            // We are removing the entire node.
            if ((node->left == nullptr) || (node->right == nullptr)) {
                // Node has zero or one child.
                Node* temp = node->left ? node->left : node->right;
                // Since we are completely deleting this node, update counters.
                total_count -= node->count;
                distinct_count--;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                // Node with two children:
                // Instead of subtracting counts here, we replace the node with its successor.
                Node* temp = minValueNode(node->right);
                // Adjust total_count: first remove the whole count of the current node and then add the count of temp.
                total_count = total_count - node->count + temp->count;
                // We do not change distinct_count here since the node remains (with a new key and count).
                node->key = temp->key;
                node->count = temp->count;
                // Remove the inorder successor from the right subtree.
                node->right = remove(node->right, temp->key, temp->count);
            }
        }
    }

    if (node == nullptr)
        return node;

    // Update the height and balance the node.
    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);

    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

template <typename T>
void AVLTree<T>::clear(Node* node) {
    if (node == nullptr)
        return;
    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::lower_bound(Node* node, const T &key) const {
    Node* ans = nullptr;
    while (node) {
        if (node->key >= key) {
            ans = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return ans;
}

template <typename T>
void AVLTree<T>::inorder(Node* node) const {
    if (node) {
        inorder(node->left);
        for (int i = 0; i < node->count; ++i)
            std::cout << node->key << " ";
        inorder(node->right);
    }
}

// Public Methods
template <typename T>
void AVLTree<T>::init_from_vector(const std::vector<T>& keys) {
    clear(root);
    root = nullptr;
    distinct_count = 0;
    total_count = 0;

    if (keys.empty()) return;

    std::vector<T> sorted_keys = keys;
    std::sort(sorted_keys.begin(), sorted_keys.end());
    root = buildFromSorted(sorted_keys, 0, sorted_keys.size() - 1);
}

template <typename T>
void AVLTree<T>::insert(const T &key) {
    root = insert(root, key, 1);
}

template <typename T>
void AVLTree<T>::insert_multiple(const T &key, int amount) {
    if (amount < 0) {  // Change from <= 0 to < 0 if you want to allow 0
        throw std::invalid_argument("Amount must be non-negative");
    }
    root = insert(root, key, amount);
}

template <typename T>
void AVLTree<T>::remove(const T &key) {
    root = remove(root, key, 1);
}

template <typename T>
void AVLTree<T>::remove_multiple(const T &key, int amount) {
    if (amount <= 0) return;
    Node* lb = lower_bound(root, key);
    if (lb == nullptr || lb->key != key) return;
    root = remove(root, key, amount);
}

template <typename T>
void AVLTree<T>::remove_all(const T &key) {
    Node* lb = lower_bound(root, key);
    if (lb == nullptr || lb->key != key) return;
    root = remove(root, key, lb->count);
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::lower_bound(const T &key) const {
    return lower_bound(root, key);
}

template <typename T>
T AVLTree<T>::min() const {
    if (root == nullptr)
        throw std::runtime_error("Tree is empty");
    Node* node = minValueNode(root);
    return node->key;
}

template <typename T>
T AVLTree<T>::max() const {
    if (root == nullptr)
        throw std::runtime_error("Tree is empty");
    Node* node = maxValueNode(root);
    return node->key;
}

template <typename T>
T AVLTree<T>::pop_min() {
    T minimum = min();
    remove(minimum);
    return minimum;
}

template <typename T>
T AVLTree<T>::pop_max() {
    T maximum = max();
    remove(maximum);
    return maximum;
}

template <typename T>
int AVLTree<T>::size() const {
    return total_count;
}

template <typename T>
void AVLTree<T>::clear() {
    clear(root);
}

template <typename T>
void AVLTree<T>::print_inorder() const {
    inorder(root);
    std::cout << std::endl;
}

#endif