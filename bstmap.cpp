// Yusuf Pisan pisan@uw.edu
// 17 Jan 2021

// BST class
// Creates a BST to store values
// Uses Node which holds the data

#include "bstmap.h"
#include <cassert>

using namespace std;

// helper for the copy constructor
void BSTMap::copyHelper(Node *n, const Node *nOriginal) {
  if (nOriginal != nullptr) {
    n->data.first = nOriginal->data.first;
    n->data.second = nOriginal->data.second;
    if (nOriginal->left != nullptr) {
      n->left = new Node();
      copyHelper(n->left, nOriginal->left);
    }
    if (nOriginal->right != nullptr) {
      n->right = new Node();
      copyHelper(n->right, nOriginal->right);
    }
  }
}

// copy constructor
BSTMap::BSTMap(const BSTMap &bst) {
  if (bst.root != nullptr) {
    this->root = new Node();
    this->root->data.first = bst.root->data.first;
    this->root->data.second = bst.root->data.second;
    if (bst.root->left != nullptr) {
      this->root->left = new Node();
      copyHelper(this->root->left, bst.root->left);
    }
    if (bst.root->right != nullptr) {
      this->root->right = new Node();
      copyHelper(this->root->right, bst.root->right);
    }
  }
}

// inserts a new node
BSTMap::Node *&BSTMap::insert(value_type l, Node *n) {
  if (n == this->root && n == nullptr) {
    root = new Node();
    root->data = l;
    treeSize++;
    return root;
  }
  int i = 0;
  while (l.first.at(i) == n->data.first.at(i) && i + 1 < l.first.size() &&
         i + 1 < n->data.first.size()) {
    i++;
  }
  if (l.first.at(i) >= n->data.first.at(i) && n->right == nullptr) {
    Node *temp = new Node();
    temp->data = l;
    temp->right = n->right;
    n->right = temp;
    treeSize++;
    return n->right;
  }
  if (l.first.at(i) < n->data.first.at(i) && n->left == nullptr) {
    Node *temp = new Node();
    temp->data = l;
    temp->left = n->left;
    n->left = temp;
    treeSize++;
    return n->left;
  }
  if (l.first.at(i) < n->data.first.at(i)) {
    return insert(l, n->left);
  }
  return insert(l, n->right);
}

// given an array of length n
// create a tree to have all items in that array
// with the minimum height (uses same helper as rebalance)
BSTMap::BSTMap(const vector<value_type> &v) {
  for (const auto &i : v) {
    insert(i, root);
  }
}

// function for inserting a vector into the tree
void BSTMap::insertVector(const vector<value_type> &v) {
  for (const auto &i : v) {
    insert(i, root);
  }
}

// helper function for clear and destructor
void BSTMap::deleteHelper(Node *n) {
  if (n->left != nullptr) {
    deleteHelper(n->left);
  }
  if (n->right != nullptr) {
    deleteHelper(n->right);
  }
  if (n != nullptr) {
    delete n;
    treeSize--;
  }
}

// destructor
BSTMap::~BSTMap() {
  if (root != nullptr) {
    deleteHelper(root);
  }
}

// delete all nodes in tree
void BSTMap::clear() {
  if (root != nullptr) {
    deleteHelper(root);
    root = nullptr;
  }
}

// true if no nodes in BST
bool BSTMap::empty() const { return this->root == nullptr; }

// Number of nodes in BST
int BSTMap::size() const { return treeSize; }

// helper for operator[] and contains, gets the node if it exists
void BSTMap::finderHelper(Node *n, const key_type &k, Node *&ret) const {
  if (n != nullptr) {
    if (n->data.first == k) {
      ret = n;
      return;
    }
    finderHelper(n->left, k, ret);
    finderHelper(n->right, k, ret);
  }
}

// true if item is in BST
bool BSTMap::contains(const key_type &key) const {
  Node *ret = nullptr;
  finderHelper(root, key, ret);
  return ret != nullptr;
}

// If k matches the key returns a reference to its value
// If k does not match any key, inserts a new element
// with that key and returns a reference to its mapped value.
BSTMap::mapped_type &BSTMap::operator[](const key_type &k) {
  Node *ret = nullptr;
  finderHelper(root, k, ret);
  if (ret != nullptr) {
    return ret->data.second;
  }
  value_type p;
  p.first = k;
  p.second = 0;
  ret = insert(p, root);
  return ret->data.second;
}

// helper for getAll
vector<BSTMap::value_type> &BSTMap::getHelper(Node *n, const key_type &k,
                                              vector<value_type> &v) const {
  if (n != nullptr) {
    getHelper(n->left, k, v);
    if (n->data.first.size() >= k.size() &&
        n->data.first.substr(0, k.size()) == k) {
      v.push_back(n->data);
    }
    getHelper(n->right, k, v);
  }
  return v;
}

// gets all pairs with k
vector<BSTMap::value_type> BSTMap::getAll(const key_type &k) const {
  vector<value_type> v;
  getHelper(root, k, v);
  return v;
}

// helper for height
void BSTMap::maxHeightHelper(int &largest, int current, const Node *n) {
  if (current > largest) {
    largest = current;
  }
  if (n != nullptr) {
    maxHeightHelper(largest, current + 1, n->left);
    maxHeightHelper(largest, current + 1, n->right);
  }
}

// 0 if empty, 1 if only root, otherwise
// height of root is max height of subtrees + 1
int BSTMap::height() const {
  int ret = 0;
  maxHeightHelper(ret, 0, root);
  return ret;
}

// height of a Node, nullptr is 0, root is 1, static, no access to 'this'
// helper function to height(), used by printVertical
int BSTMap::getHeight(const Node *n) {
  int ret = 0;
  maxHeightHelper(ret, 0, n);
  return ret;
}

// same as contains, but returns 1 or 0
// compatibility with std::map
size_t BSTMap::count(const string &k) const {
  if (contains(k)) {
    return 1;
  }
  return 0;
}

void BSTMap::inorderHelper(void visit(const value_type &item), Node *n) const {
  if (n != nullptr) {
    inorderHelper(visit, n->left);
    visit(n->data);
    inorderHelper(visit, n->right);
  }
}

// inorder traversal: left-root-right
// takes a function that takes a single parameter of type T
void BSTMap::inorder(void visit(const value_type &item)) const {
  inorderHelper(visit, root);
}

void BSTMap::preorderHelper(void visit(const value_type &item), Node *n) const {
  if (n != nullptr) {
    visit(n->data);
    preorderHelper(visit, n->left);
    preorderHelper(visit, n->right);
  }
}

// preorder traversal: root-left-right
void BSTMap::preorder(void visit(const value_type &item)) const {
  preorderHelper(visit, root);
}

void BSTMap::postorderHelper(void visit(const value_type &item),
                             Node *n) const {
  if (n != nullptr) {
    postorderHelper(visit, n->left);
    postorderHelper(visit, n->right);
    visit(n->data);
  }
}

// postorder traversal: left-right-root
void BSTMap::postorder(void visit(const value_type &item)) const {
  postorderHelper(visit, root);
}

// takes a vector, low int, and high int, and puts all pairs into a vector
// in the order that a binary search algorithm would search for them.
// doing so will make a balanced tree once they are reinserted in order.
void BSTMap::rebalanceHelper(vector<value_type> &v, int low, int high) {
  if (low + (high - low) / 2 < v.size() &&
      v[low + (high - low) / 2].second != 0) {
    insert(v[low + (high - low) / 2], root);
    v[low + (high - low) / 2].second = 0;
    rebalanceHelper(v, low, (low + (high - low) / 2) - 1);
    rebalanceHelper(v, (low + (high - low) / 2) + 1, high);
  }
}

// balance the BST by saving all nodes to a vector inorder
// and then recreating the BST from the vector
void BSTMap::rebalance() {
  vector<value_type> v = getAll("");
  clear();
  rebalanceHelper(v, 0, v.size());
}

// helper for equality checking functions
bool BSTMap::isEqual(Node *thisN, Node *otherN) const {
  if (thisN == nullptr && otherN == nullptr) {
    return true;
  }
  if (thisN == nullptr || otherN == nullptr) {
    return false;
  }
  if (thisN->data.first == otherN->data.first &&
      thisN->data.second == otherN->data.second) {
    return isEqual(thisN->left, otherN->left) &&
           isEqual(thisN->right, otherN->right);
  }
  return false;
}

// trees are equal if they have the same structure
// AND the same item values at all the nodes
bool BSTMap::operator==(const BSTMap &other) const {
  return isEqual(this->root, other.root);
}

// not == to each other
bool BSTMap::operator!=(const BSTMap &other) const {
  return !isEqual(this->root, other.root);
}