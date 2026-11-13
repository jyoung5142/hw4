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
    void rotateLeft(AVLNode<Key,Value>* n);
    void rotateRight(AVLNode<Key,Value>* n);
    void removeFix(AVLNode<Key,Value>* n, int8_t diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if(this->root_ == NULL) {

        this->root_ =
            new AVLNode<Key, Value>(
                new_item.first,
                new_item.second,
                NULL
            );

        return;
    }

    AVLNode<Key, Value>* current =
        static_cast<AVLNode<Key, Value>*>(
            this->root_
        );

    AVLNode<Key, Value>* parent = NULL;

    while(current != NULL) {

        parent = current;

        if(new_item.first == current->getKey()) {
            current->setValue(new_item.second);
            return;
        }

        else if(new_item.first < current->getKey()) {
            current = current->getLeft();
        }

        else {
            current = current->getRight();
        }
    }

    AVLNode<Key, Value>* added =
        new AVLNode<Key, Value>(
            new_item.first,
            new_item.second,
            parent
        );

    if(new_item.first < parent->getKey()) {
        parent->setLeft(added);
    }
    else {
        parent->setRight(added);
    }

    AVLNode<Key, Value>* child = added;
    AVLNode<Key, Value>* node = parent;

    while(node != NULL) {

        if(child == node->getLeft()) {
            node->updateBalance(-1);
        }
        else {
            node->updateBalance(1);
        }

        if(node->getBalance() == 0) {
            return;
        }

        else if(node->getBalance() == -1 ||
                node->getBalance() == 1)
        {
            child = node;
            node = node->getParent();
        }

        else if(node->getBalance() == -2) {

            AVLNode<Key, Value>* left =
                node->getLeft();

            if(left->getBalance() <= 0) {

                rotateRight(node);

                node->setBalance(0);
                left->setBalance(0);
            }

            else {

                AVLNode<Key, Value>* middle =
                    left->getRight();

                int8_t middleBalance =
                    middle->getBalance();

                rotateLeft(left);
                rotateRight(node);

                if(middleBalance == -1) {
                    node->setBalance(1);
                    left->setBalance(0);
                }

                else if(middleBalance == 0) {
                    node->setBalance(0);
                    left->setBalance(0);
                }

                else {
                    node->setBalance(0);
                    left->setBalance(-1);
                }

                middle->setBalance(0);
            }

            return;
        }

        else {

            AVLNode<Key, Value>* right =
                node->getRight();

            if(right->getBalance() >= 0) {

                rotateLeft(node);

                node->setBalance(0);
                right->setBalance(0);
            }

            else {

                AVLNode<Key, Value>* middle =
                    right->getLeft();

                int8_t middleBalance =
                    middle->getBalance();

                rotateRight(right);
                rotateLeft(node);

                if(middleBalance == 1) {
                    node->setBalance(-1);
                    right->setBalance(0);
                }

                else if(middleBalance == 0) {
                    node->setBalance(0);
                    right->setBalance(0);
                }

                else {
                    node->setBalance(0);
                    right->setBalance(1);
                }

                middle->setBalance(0);
            }

            return;
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* target =
        static_cast<AVLNode<Key, Value>*>(
            this->internalFind(key)
        );

    if(target == NULL) {
        return;
    }

    if(target->getLeft() != NULL &&
       target->getRight() != NULL)
    {
        AVLNode<Key, Value>* pred =
            static_cast<AVLNode<Key, Value>*>(
                BinarySearchTree<Key, Value>::predecessor(
                    target
                )
            );

        nodeSwap(target, pred);
    }

    AVLNode<Key, Value>* parent =
        target->getParent();

    AVLNode<Key, Value>* child =
        (target->getLeft() != NULL)
        ? target->getLeft()
        : target->getRight();

    int8_t diff = 0;

    if(parent != NULL) {
        diff =
            (target == parent->getLeft())
            ? 1
            : -1;
    }

    if(child != NULL) {
        child->setParent(parent);
    }

    if(parent == NULL) {
        this->root_ = child;
    }

    else if(target == parent->getLeft()) {
        parent->setLeft(child);
    }

    else {
        parent->setRight(child);
    }

    delete target;

    if(parent != NULL) {
        removeFix(parent, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(
    AVLNode<Key,Value>* n)
{
    AVLNode<Key, Value>* r =
        n->getRight();

    AVLNode<Key, Value>* p =
        n->getParent();

    AVLNode<Key, Value>* middle =
        r->getLeft();

    if(p == NULL) {
        this->root_ = r;
    }

    else if(n == p->getLeft()) {
        p->setLeft(r);
    }

    else {
        p->setRight(r);
    }

    r->setParent(p);

    r->setLeft(n);
    n->setParent(r);

    n->setRight(middle);

    if(middle != NULL) {
        middle->setParent(n);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(
    AVLNode<Key,Value>* n)
{
    AVLNode<Key, Value>* l =
        n->getLeft();

    AVLNode<Key, Value>* p =
        n->getParent();

    AVLNode<Key, Value>* middle =
        l->getRight();

    if(p == NULL) {
        this->root_ = l;
    }

    else if(n == p->getLeft()) {
        p->setLeft(l);
    }

    else {
        p->setRight(l);
    }

    l->setParent(p);

    l->setRight(n);
    n->setParent(l);

    n->setLeft(middle);

    if(middle != NULL) {
        middle->setParent(n);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(
    AVLNode<Key,Value>* n,
    int8_t diff)
{
    while(n != NULL) {

        AVLNode<Key, Value>* parent =
            n->getParent();

        int8_t nextDiff = 0;

        if(parent != NULL) {
            nextDiff =
                (n == parent->getLeft())
                ? 1
                : -1;
        }

        int8_t nextBalance =
            n->getBalance() + diff;

        if(nextBalance == -2) {

            AVLNode<Key, Value>* left =
                n->getLeft();

            int8_t leftBalance =
                left->getBalance();

            if(leftBalance <= 0) {

                rotateRight(n);

                if(leftBalance == 0) {
                    n->setBalance(-1);
                    left->setBalance(1);
                    return;
                }

                n->setBalance(0);
                left->setBalance(0);
            }

            else {

                AVLNode<Key, Value>* middle =
                    left->getRight();

                int8_t middleBalance =
                    middle->getBalance();

                rotateLeft(left);
                rotateRight(n);

                if(middleBalance == -1) {
                    n->setBalance(1);
                    left->setBalance(0);
                }

                else if(middleBalance == 0) {
                    n->setBalance(0);
                    left->setBalance(0);
                }

                else {
                    n->setBalance(0);
                    left->setBalance(-1);
                }

                middle->setBalance(0);
            }

            n = parent;
            diff = nextDiff;
        }

        else if(nextBalance == 2) {

            AVLNode<Key, Value>* right =
                n->getRight();

            int8_t rightBalance =
                right->getBalance();

            if(rightBalance >= 0) {

                rotateLeft(n);

                if(rightBalance == 0) {
                    n->setBalance(1);
                    right->setBalance(-1);
                    return;
                }

                n->setBalance(0);
                right->setBalance(0);
            }

            else {

                AVLNode<Key, Value>* middle =
                    right->getLeft();

                int8_t middleBalance =
                    middle->getBalance();

                rotateRight(right);
                rotateLeft(n);

                if(middleBalance == 1) {
                    n->setBalance(-1);
                    right->setBalance(0);
                }

                else if(middleBalance == 0) {
                    n->setBalance(0);
                    right->setBalance(0);
                }

                else {
                    n->setBalance(0);
                    right->setBalance(1);
                }

                middle->setBalance(0);
            }

            n = parent;
            diff = nextDiff;
        }

        else if(nextBalance == 0) {

            n->setBalance(0);

            n = parent;
            diff = nextDiff;
        }

        else {

            n->setBalance(nextBalance);
            return;
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
