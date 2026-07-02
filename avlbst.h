#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    // Add helper functions here
private:
    int height(AVLNode<Key, Value>* node) const;
    void fixBalance(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* x);
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* y);
    AVLNode<Key, Value>* rebalanceSubtree(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rebalanceInsert(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rebalanceRemove(AVLNode<Key, Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
 /*
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    Node<Key, Value>* cur = this->root_;
    Node<Key, Value>* parent = nullptr;

    while (cur != nullptr) {
        parent = cur;
        if (new_item.first < cur->getKey()) {
            cur = cur->getLeft();
        } else if (new_item.first > cur->getKey()) {
            cur = cur->getRight();
        } else {
            cur->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key, Value>* newNode =
        new AVLNode<Key, Value>(new_item.first, new_item.second,
                                static_cast<AVLNode<Key, Value>*>(parent));

    if (new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }

    this->root_ = rebalanceSubtree(static_cast<AVLNode<Key, Value>*>(this->root_));
    if (this->root_ != nullptr) {
        this->root_->setParent(nullptr);
    }
}
*/
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
/*
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
  BinarySearchTree<Key, Value>::remove(key);

    if (this->root_ != nullptr) {
        this->root_ = rebalanceSubtree(static_cast<AVLNode<Key, Value>*>(this->root_));
        this->root_->setParent(nullptr);
    }
}
*/
template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value>* node) const
{
    if (node == nullptr) {
        return 0;
    }
    int leftH = height(node->getLeft());
    int rightH = height(node->getRight());
    return 1 + std::max(leftH, rightH);
}

template<class Key, class Value>
void AVLTree<Key, Value>::fixBalance(AVLNode<Key, Value>* node)
{
    if (node == nullptr) {
        return;
    }
    node->setBalance(height(node->getRight()) - height(node->getLeft()));
}
/*
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x)
{
    AVLNode<Key, Value>* y = x->getRight();
    AVLNode<Key, Value>* beta = y->getLeft();

    y->setParent(x->getParent());
    if (x->getParent() != nullptr) {
        if (x->getParent()->getLeft() == x) {
            x->getParent()->setLeft(y);
        } else {
            x->getParent()->setRight(y);
        }
    }

    y->setLeft(x);
    x->setParent(y);
    x->setRight(beta);
    if (beta != nullptr) {
        beta->setParent(x);
    }

    fixBalance(x);
    fixBalance(y);
    return y;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* y)
{
    AVLNode<Key, Value>* x = y->getLeft();
    AVLNode<Key, Value>* beta = x->getRight();

    x->setParent(y->getParent());
    if (y->getParent() != nullptr) {
        if (y->getParent()->getLeft() == y) {
            y->getParent()->setLeft(x);
        } else {
            y->getParent()->setRight(x);
        }
    }

    x->setRight(y);
    y->setParent(x);
    y->setLeft(beta);
    if (beta != nullptr) {
        beta->setParent(y);
    }

    fixBalance(y);
    fixBalance(x);
    return x;
}
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rebalanceSubtree(AVLNode<Key, Value>* node)
{
    if (node == nullptr) {
        return nullptr;
    }

    rebalanceSubtree(node->getLeft());
    rebalanceSubtree(node->getRight());

    fixBalance(node);

    int bal = node->getBalance();

    if (bal < -1) {
        if (height(node->getLeft()->getRight()) > height(node->getLeft()->getLeft())) {
            rotateLeft(node->getLeft());
        }
        return rotateRight(node);
    }

    if (bal > 1) {
        if (height(node->getRight()->getLeft()) > height(node->getRight()->getRight())) {
            rotateRight(node->getRight());
        }
        return rotateLeft(node);
    }

    return node;
}
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x)
{
    AVLNode<Key, Value>* y = x->getRight();
    AVLNode<Key, Value>* beta = y->getLeft();
    AVLNode<Key, Value>* parent = x->getParent();

    y->setParent(parent);
    if (parent != nullptr) {
        if (parent->getLeft() == x) {
            parent->setLeft(y);
        } else {
            parent->setRight(y);
        }
    }

    y->setLeft(x);
    x->setParent(y);

    x->setRight(beta);
    if (beta != nullptr) {
        beta->setParent(x);
    }

    return y;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* y)
{
    AVLNode<Key, Value>* x = y->getLeft();
    AVLNode<Key, Value>* beta = x->getRight();
    AVLNode<Key, Value>* parent = y->getParent();

    x->setParent(parent);
    if (parent != nullptr) {
        if (parent->getLeft() == y) {
            parent->setLeft(x);
        } else {
            parent->setRight(x);
        }
    }

    x->setRight(y);
    y->setParent(x);

    y->setLeft(beta);
    if (beta != nullptr) {
        beta->setParent(y);
    }

    return x;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rebalanceInsert(AVLNode<Key, Value>* node)
{
    if (node->getBalance() == -2) {
        AVLNode<Key, Value>* left = node->getLeft();

        if (left->getBalance() <= 0) {
            AVLNode<Key, Value>* newRoot = rotateRight(node);
            newRoot->setBalance(0);
            newRoot->getRight()->setBalance(0);
            return newRoot;
        } else {
            AVLNode<Key, Value>* x = left->getRight();
            int8_t xb = x->getBalance();

            rotateLeft(left);
            AVLNode<Key, Value>* newRoot = rotateRight(node);

            newRoot->setBalance(0);
            if (xb == -1) {
                newRoot->getLeft()->setBalance(0);
                newRoot->getRight()->setBalance(1);
            } else if (xb == 0) {
                newRoot->getLeft()->setBalance(0);
                newRoot->getRight()->setBalance(0);
            } else {
                newRoot->getLeft()->setBalance(-1);
                newRoot->getRight()->setBalance(0);
            }
            return newRoot;
        }
    } else { // node->getBalance() == 2
        AVLNode<Key, Value>* right = node->getRight();

        if (right->getBalance() >= 0) {
            AVLNode<Key, Value>* newRoot = rotateLeft(node);
            newRoot->setBalance(0);
            newRoot->getLeft()->setBalance(0);
            return newRoot;
        } else {
            AVLNode<Key, Value>* x = right->getLeft();
            int8_t xb = x->getBalance();

            rotateRight(right);
            AVLNode<Key, Value>* newRoot = rotateLeft(node);

            newRoot->setBalance(0);
            if (xb == 1) {
                newRoot->getLeft()->setBalance(-1);
                newRoot->getRight()->setBalance(0);
            } else if (xb == 0) {
                newRoot->getLeft()->setBalance(0);
                newRoot->getRight()->setBalance(0);
            } else {
                newRoot->getLeft()->setBalance(0);
                newRoot->getRight()->setBalance(1);
            }
            return newRoot;
        }
    }
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rebalanceRemove(AVLNode<Key, Value>* node)
{
    if (node->getBalance() == -2) {
        AVLNode<Key, Value>* left = node->getLeft();

        if (left->getBalance() <= 0) {
            int8_t lb = left->getBalance();
            AVLNode<Key, Value>* newRoot = rotateRight(node);

            if (lb == 0) {
                newRoot->setBalance(1);
                newRoot->getRight()->setBalance(-1);
            } else {
                newRoot->setBalance(0);
                newRoot->getRight()->setBalance(0);
            }
            return newRoot;
        } else {
            AVLNode<Key, Value>* x = left->getRight();
            int8_t xb = x->getBalance();

            rotateLeft(left);
            AVLNode<Key, Value>* newRoot = rotateRight(node);

            newRoot->setBalance(0);
            if (xb == -1) {
                newRoot->getLeft()->setBalance(0);
                newRoot->getRight()->setBalance(1);
            } else if (xb == 0) {
                newRoot->getLeft()->setBalance(0);
                newRoot->getRight()->setBalance(0);
            } else {
                newRoot->getLeft()->setBalance(-1);
                newRoot->getRight()->setBalance(0);
            }
            return newRoot;
        }
    } else { // node->getBalance() == 2
        AVLNode<Key, Value>* right = node->getRight();

        if (right->getBalance() >= 0) {
            int8_t rb = right->getBalance();
            AVLNode<Key, Value>* newRoot = rotateLeft(node);

            if (rb == 0) {
                newRoot->setBalance(-1);
                newRoot->getLeft()->setBalance(1);
            } else {
                newRoot->setBalance(0);
                newRoot->getLeft()->setBalance(0);
            }
            return newRoot;
        } else {
            AVLNode<Key, Value>* x = right->getLeft();
            int8_t xb = x->getBalance();

            rotateRight(right);
            AVLNode<Key, Value>* newRoot = rotateLeft(node);

            newRoot->setBalance(0);
            if (xb == 1) {
                newRoot->getLeft()->setBalance(-1);
                newRoot->getRight()->setBalance(0);
            } else if (xb == 0) {
                newRoot->getLeft()->setBalance(0);
                newRoot->getRight()->setBalance(0);
            } else {
                newRoot->getLeft()->setBalance(0);
                newRoot->getRight()->setBalance(1);
            }
            return newRoot;
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    AVLNode<Key, Value>* cur = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while (cur != nullptr) {
        parent = cur;

        if (new_item.first < cur->getKey()) {
            cur = cur->getLeft();
        } else if (new_item.first > cur->getKey()) {
            cur = cur->getRight();
        } else {
            cur->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key, Value>* newNode =
        new AVLNode<Key, Value>(new_item.first, new_item.second, parent);

    if (new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }

    AVLNode<Key, Value>* child = newNode;
    cur = parent;

    while (cur != nullptr) {
        if (child == cur->getLeft()) {
            cur->updateBalance(-1);
        } else {
            cur->updateBalance(1);
        }

        if (cur->getBalance() == 0) {
            break;
        } else if (cur->getBalance() == -1 || cur->getBalance() == 1) {
            child = cur;
            cur = cur->getParent();
        } else {
            AVLNode<Key, Value>* newRoot = rebalanceInsert(cur);
            if (newRoot->getParent() == nullptr) {
                this->root_ = newRoot;
                this->root_->setParent(nullptr);
            }
            break;
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->root_);

    while (node != nullptr) {
        if (key < node->getKey()) {
            node = node->getLeft();
        } else if (key > node->getKey()) {
            node = node->getRight();
        } else {
            break;
        }
    }

    if (node == nullptr) {
        return;
    }

    // If the node has two children, swap with predecessor first.
    if (node->getLeft() != nullptr && node->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = node->getLeft();
        while (pred->getRight() != nullptr) {
            pred = pred->getRight();
        }
        this->nodeSwap(node, pred);
    }

    AVLNode<Key, Value>* child = (node->getLeft() != nullptr) ? node->getLeft() : node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();

    // Save which side the removed node was on before unlinking it.
    bool removedFromLeft = false;
    if (parent != nullptr && parent->getLeft() == node) {
        removedFromLeft = true;
    }

    if (child != nullptr) {
        child->setParent(parent);
    }

    if (parent == nullptr) {
        this->root_ = child;
        if (this->root_ != nullptr) {
            this->root_->setParent(nullptr);
        }
        delete node;
        return;
    }

    if (removedFromLeft) {
        parent->setLeft(child);
    } else {
        parent->setRight(child);
    }

    delete node;

    AVLNode<Key, Value>* cur = parent;

    while (cur != nullptr) {
        if (removedFromLeft) {
            cur->updateBalance(1);
        } else {
            cur->updateBalance(-1);
        }

        if (cur->getBalance() == 1 || cur->getBalance() == -1) {
            break;
        }

        if (cur->getBalance() == 0) {
            removedFromLeft = (cur->getParent() != nullptr && cur->getParent()->getLeft() == cur);
            cur = cur->getParent();
            continue;
        }

        AVLNode<Key, Value>* newRoot = rebalanceRemove(cur);

        if (newRoot->getParent() == nullptr) {
            this->root_ = newRoot;
            this->root_->setParent(nullptr);
        }

        removedFromLeft = (newRoot->getParent() != nullptr && newRoot->getParent()->getLeft() == newRoot);
        cur = newRoot->getParent();
    }
}
#endif
