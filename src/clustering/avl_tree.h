#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <memory>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

template <typename K, typename V>
class AVLNode
{
public:
    K key;
    V value;
    int height;
    shared_ptr<AVLNode<K, V>> left;
    shared_ptr<AVLNode<K, V>> right;
    AVLNode(const K &k, const V &v) : key(k), value(v), height(1) {}
};

template <typename K, typename V>
class AVLTree
{
private:
    shared_ptr<AVLNode<K, V>> root;
    int getHeight(shared_ptr<AVLNode<K, V>> node);
    int getBalance(shared_ptr<AVLNode<K, V>> node);
    void updateHeight(shared_ptr<AVLNode<K, V>> node);
    shared_ptr<AVLNode<K, V>> rotateRight(shared_ptr<AVLNode<K, V>> y);
    shared_ptr<AVLNode<K, V>> rotateLeft(shared_ptr<AVLNode<K, V>> x);
    shared_ptr<AVLNode<K, V>> insertHelper(shared_ptr<AVLNode<K, V>> node, const K &key, const V &value);
    shared_ptr<AVLNode<K, V>> searchHelper(shared_ptr<AVLNode<K, V>> node, const K &key);
    void inorderHelper(shared_ptr<AVLNode<K, V>> node, vector<pair<K, V>> &res);

public:
    AVLTree();
    void insert(const K &key, const V &value);
    V *search(const K &key);
    vector<pair<K, V>> inorderTraversal();
};

#endif
