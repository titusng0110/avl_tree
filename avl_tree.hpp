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
    Node* minValueNode(Node* node) const;
    Node* maxValueNode(Node* node) const;
    Node* remove(Node* node, const T &key, int amount);
    void clear(Node* node);
    Node* lower_bound(Node* node, const T &key) const;
    void inorder(Node* node, std::vector<T>& result) const;
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
    int count(const T& key) const;
    bool contains(const T& key) const;
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
    clear();
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

    // Safer mid calculation
    int mid = start + (end - start) / 2;
    
    // Count duplicates
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
    
    // Check balance factor and rotate if necessary
    int balance = getBalance(node);
    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0)
    return rotateRight(node);

    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0)
    return rotateLeft(node);

    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
    
}

template <typename T>
int AVLTree<T>::getBalance(Node* node) const {
    return (node == nullptr) ? 0 : height(node->left) - height(node->right);
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotateRight(Node* y) {
    if (y == nullptr) {
        // Handle error or return nullptr
        std::cout<<"node is nullptr during rotateRight"<<std::endl;
        return y;
    }
    else if (y->left == nullptr) {
        std::cout<<"node->left is nullptr during rotateRight"<<std::endl;
        return y;
    }

    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + std::max(height(y->left), height(y->right));
    x->height = 1 + std::max(height(x->left), height(x->right));

    return x;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotateLeft(Node* x) {
    if (x == nullptr) {
        // Handle error or return nullptr
        std::cout<<"node is nullptr during rotateLeft"<<std::endl;
        return x;
    }
    else if (x->right == nullptr) {
        std::cout<<"node->right is nullptr during rotateLeft"<<std::endl;
        return x;
    }

    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));

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
typename AVLTree<T>::Node* AVLTree<T>::minValueNode(Node* node) const {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::maxValueNode(Node* node) const {
    Node* current = node;
    while (current->right != nullptr)
        current = current->right;
    return current;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::remove(Node* node, const T &key, int amount) {
    if (node == nullptr)
        return node;

    // Traverse to the node to be deleted.
    if (key < node->key)
        node->left = remove(node->left, key, amount);
    else if (key > node->key)
        node->right = remove(node->right, key, amount);
    else {
        // Found the node with the target key.
        if (amount < node->count) {
            // Only remove part of the count.
            node->count -= amount;
            total_count -= amount;
            return node;
        } else {
            // Remove the entire node.
            
            // Case 1: node has 0 or 1 child.
            if (node->left == nullptr || node->right == nullptr) {
                total_count -= node->count;
                distinct_count--;
                Node* temp = node->left ? node->left : node->right;
                if (temp == nullptr) {
                    // No children: free the node and return nullptr.
                    delete node;
                    return nullptr;
                } else {
                    // One child: replace node with its child.
                    Node* oldNode = node;
                    node = temp;
                    delete oldNode;
                }
            }
            // Case 2: node has two children.
            else {
                // Find inorder successor.
                Node* temp = minValueNode(node->right);
                // Adjust total_count:
                total_count = total_count - node->count + temp->count;
                // Copy the successor's data to the current node.
                node->key = temp->key;
                node->count = temp->count;
                // Remove the inorder successor.
                node->right = remove(node->right, temp->key, temp->count);
            }
        }
    }

    if (node == nullptr)
        return node;

    // Update the height.
    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Rebalance if needed.
    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0)
    return rotateLeft(node);

    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

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
void AVLTree<T>::inorder(Node* node, std::vector<T>& result) const {
    if (node) {
        inorder(node->left, result);
        for (int i = 0; i < node->count; ++i) {
            result.push_back(node->key);
        }
        inorder(node->right, result);
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
    if (amount < 0) {
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
int AVLTree<T>::count(const T& key) const {
    Node* node = lower_bound(root, key);
    if (node && node->key == key) {
        return node->count;
    }
    return 0;
}

template <typename T>
bool AVLTree<T>::contains(const T& key) const {
    Node* node = lower_bound(root, key);
    return node != nullptr && node->key == key;
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
    root = nullptr;
    distinct_count = 0;
    total_count = 0;
}

template <typename T>
void AVLTree<T>::print_inorder() const {
    std::vector<T> result;
    inorder(root, result);
    for (const T& element : result) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

#endif