#include "avl_tree.h"
#include <string>
using namespace std;

template <typename K, typename V>
AVLTree<K, V>::AVLTree() : root(nullptr) {}

template <typename K, typename V>
int AVLTree<K, V>::getHeight(shared_ptr<AVLNode<K, V>> node)
{
    return node ? node->height : 0;
}

template <typename K, typename V>
int AVLTree<K, V>::getBalance(shared_ptr<AVLNode<K, V>> node)
{
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

template <typename K, typename V>
void AVLTree<K, V>::updateHeight(shared_ptr<AVLNode<K, V>> node)
{
    if (node)
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

template <typename K, typename V>
shared_ptr<AVLNode<K, V>> AVLTree<K, V>::rotateRight(shared_ptr<AVLNode<K, V>> y)
{
    auto x = y->left;
    auto T2 = x->right;
    x->right = y;
    y->left = T2;
    updateHeight(y);
    updateHeight(x);
    return x;
}

template <typename K, typename V>
shared_ptr<AVLNode<K, V>> AVLTree<K, V>::rotateLeft(shared_ptr<AVLNode<K, V>> x)
{
    auto y = x->right;
    auto T2 = y->left;
    y->left = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}

template <typename K, typename V>
shared_ptr<AVLNode<K, V>> AVLTree<K, V>::insertHelper(shared_ptr<AVLNode<K, V>> node, const K &key, const V &value)
{
    if (!node)
        return make_shared<AVLNode<K, V>>(key, value);
    if (key < node->key)
        node->left = insertHelper(node->left, key, value);
    else if (key > node->key)
        node->right = insertHelper(node->right, key, value);
    else
    {
        node->value = value;
        return node;
    }
    updateHeight(node);
    int balance = getBalance(node);
    if (balance > 1 && key < node->left->key)
        return rotateRight(node);
    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);
    if (balance > 1 && key > node->left->key)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && key < node->right->key)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

template <typename K, typename V>
shared_ptr<AVLNode<K, V>> AVLTree<K, V>::searchHelper(shared_ptr<AVLNode<K, V>> node, const K &key)
{
    if (!node || node->key == key)
        return node;
    if (key < node->key)
        return searchHelper(node->left, key);
    return searchHelper(node->right, key);
}

template <typename K, typename V>
void AVLTree<K, V>::inorderHelper(shared_ptr<AVLNode<K, V>> node, vector<pair<K, V>> &res)
{
    if (!node)
        return;
    inorderHelper(node->left, res);
    res.push_back({node->key, node->value});
    inorderHelper(node->right, res);
}

template <typename K, typename V>
void AVLTree<K, V>::insert(const K &key, const V &value) { root = insertHelper(root, key, value); }

template <typename K, typename V>
V *AVLTree<K, V>::search(const K &key)
{
    auto node = searchHelper(root, key);
    return node ? &node->value : nullptr;
}

template <typename K, typename V>
vector<pair<K, V>> AVLTree<K, V>::inorderTraversal()
{
    vector<pair<K, V>> res;
    inorderHelper(root, res);
    return res;
}

template class AVLTree<string, int>;
template class AVLTree<string, unsigned int>;
