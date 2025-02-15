#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>

template <typename T>
class AVLTree
{
private:
    struct Node
    {
        T key;
        short height;
        int count;
        Node *left;
        Node *right;
        Node(const T &k, int cnt = 1)
            : key(k), height(1), count(cnt), left(nullptr), right(nullptr) {}
    };

    Node *root;
    Node *min_node;
    Node *max_node;
    int distinct_count;
    int total_count;

    short height(Node *node) const;
    Node *buildFromSorted(const std::vector<T> &keys, int start, int end);
    int getBalance(Node *node) const;
    Node *rotateRight(Node *y);
    Node *rotateLeft(Node *x);
    Node *insert(Node *node, const T &key, int amount);
    Node *getMinNode(Node *node) const;
    Node *getMaxNode(Node *node) const;
    void updateMinNode();
    void updateMaxNode();
    Node *remove(Node *node, const T &key, int amount);
    void clear(Node *node);
    Node *lower_bound(Node *node, const T &key) const;
    void inorder(Node *node, std::vector<T> &result) const;

public:
    AVLTree();
    ~AVLTree();
    void init_from_vector(const std::vector<T> &keys);
    void insert(const T &key);
    void insert_multiple(const T &key, int amount);
    template <typename Iterator>
    void bulk_insert(Iterator begin, Iterator end);
    void remove_multiple(const T &key, int amount);
    void remove_all(const T &key);
    void remove(const T &key);
    Node *lower_bound(const T &key) const;
    int count(const T &key) const;
    bool contains(const T &key) const;
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
AVLTree<T>::~AVLTree()
{
    clear();
}

// Private Helper Methods
template <typename T>
short AVLTree<T>::height(Node *node) const
{
    return (node == nullptr) ? 0 : node->height;
}

template <typename T>
typename AVLTree<T>::Node *AVLTree<T>::buildFromSorted(const std::vector<T> &keys, int start, int end)
{
    if (start > end)
        return nullptr;

    // Safer mid calculation
    int mid = start + (end - start) / 2;

    // Count duplicates
    int count = 1;
    int i = mid - 1;
    while (i >= start && keys[i] == keys[mid])
    {
        count++;
        i--;
    }
    i = mid + 1;
    while (i <= end && keys[i] == keys[mid])
    {
        count++;
        i++;
    }

    Node *node = new Node(keys[mid], count);
    distinct_count++;
    total_count += count;

    int leftEnd = mid - 1;
    while (leftEnd >= start && keys[leftEnd] == keys[mid])
        leftEnd--;

    int rightStart = mid + 1;
    while (rightStart <= end && keys[rightStart] == keys[mid])
        rightStart++;

    node->left = buildFromSorted(keys, start, leftEnd);
    node->right = buildFromSorted(keys, rightStart, end);

    node->height = 1 + std::max(height(node->left), height(node->right));

    // Check balance factor and rotate if necessary
    int balance = getBalance(node);
    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0)
        node = rotateRight(node);

    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0)
        node = rotateLeft(node);

    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        node = rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        node = rotateLeft(node);
    }

    return node;
}

template <typename T>
int AVLTree<T>::getBalance(Node *node) const
{
    return (node == nullptr) ? 0 : height(node->left) - height(node->right);
}

template <typename T>
typename AVLTree<T>::Node *AVLTree<T>::rotateRight(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + std::max(height(y->left), height(y->right));
    x->height = 1 + std::max(height(x->left), height(x->right));

    return x;
}

template <typename T>
typename AVLTree<T>::Node *AVLTree<T>::rotateLeft(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));

    return y;
}

template <typename T>
typename AVLTree<T>::Node *AVLTree<T>::insert(Node *node, const T &key, int amount)
{
    if (node == nullptr)
    {
        Node *newNode = new Node(key, amount);
        distinct_count++;
        total_count += amount;

        if (!min_node || key < min_node->key)
            min_node = newNode;
        if (!max_node || key > max_node->key)
            max_node = newNode;

        return newNode;
    }

    if (key < node->key)
        node->left = insert(node->left, key, amount);
    else if (key > node->key)
        node->right = insert(node->right, key, amount);
    else
    {
        node->count += amount;
        total_count += amount;
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        node = rotateRight(node);
    else if (balance < -1 && key > node->right->key)
        node = rotateLeft(node);
    else if (balance > 1 && key > node->left->key)
    {
        node->left = rotateLeft(node->left);
        node = rotateRight(node);
    }
    else if (balance < -1 && key < node->right->key)
    {
        node->right = rotateRight(node->right);
        node = rotateLeft(node);
    }

    // If the node was previously min or max and was rotated,
    // we need to update the cached pointers
    if (min_node && key < min_node->key)
        min_node = node;
    if (max_node && key > max_node->key)
        max_node = node;

    return node;
}

template <typename T>
typename AVLTree<T>::Node *AVLTree<T>::getMinNode(Node *node) const
{
    Node *current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

template <typename T>
typename AVLTree<T>::Node *AVLTree<T>::getMaxNode(Node *node) const
{
    Node *current = node;
    while (current->right != nullptr)
        current = current->right;
    return current;
}

template <typename T>
void AVLTree<T>::updateMinNode()
{
    min_node = (root == nullptr) ? nullptr : getMinNode(root);
}

template <typename T>
void AVLTree<T>::updateMaxNode()
{
    max_node = (root == nullptr) ? nullptr : getMaxNode(root);
}

template <typename T>
typename AVLTree<T>::Node *AVLTree<T>::remove(Node *node, const T &key, int amount)
{
    if (node == nullptr)
        return node;

    // Traverse to the node to be deleted.
    if (key < node->key)
        node->left = remove(node->left, key, amount);
    else if (key > node->key)
        node->right = remove(node->right, key, amount);
    else
    {
        // Found the node with the target key.
        if (amount < node->count)
        {
            // Only remove part of the count.
            node->count -= amount;
            total_count -= amount;
            return node;
        }
        else
        {
            // Remove the entire node.

            // Case 1: node has 0 or 1 child.
            if (node->left == nullptr || node->right == nullptr)
            {
                total_count -= node->count;
                distinct_count--;
                Node *temp = node->left ? node->left : node->right;
                if (temp == nullptr)
                {
                    // No children: free the node and return nullptr.
                    delete node;
                    return nullptr;
                }
                else
                {
                    // One child: replace node with its child.
                    Node *oldNode = node;
                    node = temp;
                    delete oldNode;
                }
            }
            // Case 2: node has two children.
            else
            {
                // Find inorder successor.
                Node *temp = getMinNode(node->right);
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
        node = rotateRight(node);

    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0)
        node = rotateLeft(node);

    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        node = rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        node = rotateLeft(node);
    }

    return node;
}

template <typename T>
void AVLTree<T>::clear(Node *node)
{
    if (node == nullptr)
        return;
    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T>
typename AVLTree<T>::Node *AVLTree<T>::lower_bound(Node *node, const T &key) const
{
    Node *ans = nullptr;
    while (node)
    {
        if (node->key >= key)
        {
            ans = node;
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    return ans;
}

template <typename T>
void AVLTree<T>::inorder(Node *node, std::vector<T> &result) const
{
    if (node)
    {
        inorder(node->left, result);
        for (int i = 0; i < node->count; ++i)
        {
            result.push_back(node->key);
        }
        inorder(node->right, result);
    }
}

// Public Methods
template <typename T>
void AVLTree<T>::init_from_vector(const std::vector<T> &keys)
{
    clear();
    if (keys.empty())
        return;

    std::vector<T> sorted_keys = keys;
    std::sort(sorted_keys.begin(), sorted_keys.end());
    root = buildFromSorted(sorted_keys, 0, sorted_keys.size() - 1);

    updateMinNode();
    updateMaxNode();
}

template <typename T>
void AVLTree<T>::insert(const T &key)
{
    root = insert(root, key, 1);
}

template <typename T>
void AVLTree<T>::insert_multiple(const T &key, int amount)
{
    if (amount < 0)
    {
        throw std::invalid_argument("Amount must be non-negative");
    }
    root = insert(root, key, amount);
}

template <typename T>
template <typename Iterator>
void AVLTree<T>::bulk_insert(Iterator begin, Iterator end) {
    // If bulk is small compared to tree size, do individual insertions
    size_t bulk_size = std::distance(begin, end);
    if (bulk_size < size()) {
        for (Iterator it = begin; it != end; ++it) {
            insert(*it);
        }
        return;
    }

    // Get current elements in sorted order
    std::vector<T> current;
    inorder(root, current);
    
    // Sort the bulk elements
    std::vector<T> bulk_elements(begin, end);
    std::sort(bulk_elements.begin(), bulk_elements.end());
    
    // Merge current and bulk elements
    std::vector<T> merged;
    merged.reserve(current.size() + bulk_size);
    std::merge(current.begin(), current.end(),
               bulk_elements.begin(), bulk_elements.end(),
               std::back_inserter(merged));
    
    // Rebuild tree
    clear();
    root = buildFromSorted(merged, 0, merged.size() - 1);
    updateMinNode();
    updateMaxNode();
}

template <typename T>
void AVLTree<T>::remove(const T &key)
{
    root = remove(root, key, 1);
    updateMinNode();
    updateMaxNode();
}

template <typename T>
void AVLTree<T>::remove_multiple(const T &key, int amount)
{
    if (amount <= 0)
        return;
    Node *lb = lower_bound(root, key);
    if (lb == nullptr || lb->key != key)
        return;
    root = remove(root, key, amount);
    updateMinNode();
    updateMaxNode();
}

template <typename T>
void AVLTree<T>::remove_all(const T &key)
{
    Node *lb = lower_bound(root, key);
    if (lb == nullptr || lb->key != key)
        return;
    root = remove(root, key, lb->count);
    updateMinNode();
    updateMaxNode();
}

template <typename T>
typename AVLTree<T>::Node *AVLTree<T>::lower_bound(const T &key) const
{
    return lower_bound(root, key);
}

template <typename T>
int AVLTree<T>::count(const T &key) const
{
    Node *node = lower_bound(root, key);
    if (node && node->key == key)
    {
        return node->count;
    }
    return 0;
}

template <typename T>
bool AVLTree<T>::contains(const T &key) const
{
    Node *node = lower_bound(root, key);
    return node != nullptr && node->key == key;
}

template <typename T>
T AVLTree<T>::min() const
{
    if (!min_node)
        throw std::runtime_error("Tree is empty");
    return min_node->key;
}

template <typename T>
T AVLTree<T>::max() const
{
    if (!max_node)
        throw std::runtime_error("Tree is empty");
    return max_node->key;
}

template <typename T>
T AVLTree<T>::pop_min()
{
    if (!min_node)
        throw std::runtime_error("Tree is empty");
    T minimum = min_node->key;
    remove(minimum);
    return minimum;
}

template <typename T>
T AVLTree<T>::pop_max()
{
    if (!max_node)
        throw std::runtime_error("Tree is empty");
    T maximum = max_node->key;
    remove(maximum);
    return maximum;
}

template <typename T>
int AVLTree<T>::size() const
{
    return total_count;
}

template <typename T>
void AVLTree<T>::clear()
{
    clear(root);
    root = nullptr;
    min_node = nullptr;
    max_node = nullptr;
    distinct_count = 0;
    total_count = 0;
}

template <typename T>
void AVLTree<T>::print_inorder() const
{
    std::vector<T> result;
    inorder(root, result);
    for (const T &element : result)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

#endif