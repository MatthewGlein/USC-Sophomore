#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "bst.h"

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus 
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
    int getHeight() const;
    void setHeight(int height);

    // Getters for parent, left, and right. These need to be redefined since they 
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int mHeight;
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a height of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
    : Node<Key, Value>(key, value, parent)
    , mHeight(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* Getter function for the height. 
*/
template<typename Key, typename Value>
int AVLNode<Key, Value>::getHeight() const
{
    return mHeight;
}

/**
* Setter function for the height. 
*/
template<typename Key, typename Value>
void AVLNode<Key, Value>::setHeight(int height)
{
    mHeight = height;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}
/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods. 
    virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
    void remove(const Key& key);
    AVLNode<Key, Value>* getRoot();

private:
    int height(AVLNode<Key, Value> *node);
	/* Helper functions are strongly encouraged to help separate the problem
	   into smaller pieces. You should not need additional data members. */
    void correctHeight(AVLNode<Key, Value> *node);

    int balanceFactor(AVLNode<Key, Value> *node);

    void rotateRightRight(AVLNode<Key, Value> *node);

    void rotateLeftLeft(AVLNode<Key, Value> *node);

    void rotateRightLeft(AVLNode<Key, Value> *node);

    void rotateLeftRight(AVLNode<Key, Value> *node);

    void balanceTree(AVLNode<Key, Value> *node);

    AVLNode<Key, Value>* recursiveInsert(AVLNode<Key, Value> *node, AVLNode<Key, Value> *newNode);

    AVLNode<Key, Value>* findMin(AVLNode<Key, Value> *node);

    AVLNode<Key, Value>* removeMin(AVLNode<Key, Value> *node);

    void updateHeights(AVLNode<Key, Value> *node);

};

/*
--------------------------------------------
Begin implementations for the AVLTree class.
--------------------------------------------
*/

/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
    // TODO
    AVLNode<Key, Value> *newNode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
    if(this->mRoot == NULL){
        this->mRoot = newNode;
    }
    else{
        AVLNode<Key, Value> *root = static_cast<AVLNode<Key,Value>*>(this->mRoot);
        newNode = recursiveInsert(root, newNode);
        updateHeights(root);
        balanceTree(newNode);
        //std::cerr<<newNode->getValue()<<" ";
    }
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::recursiveInsert(AVLNode<Key, Value> *node, AVLNode<Key, Value> *newNode){
    AVLNode<Key, Value> *tempRoot = static_cast<AVLNode<Key,Value>*>(this->mRoot);
    if(node == NULL){
        node = new AVLNode<Key, Value>(newNode->getKey(), newNode->getValue(), node->getParent());
        return node;
    }
    if(newNode->getKey() < node->getKey()){
        if(node->getLeft() == NULL){
            node->setLeft(newNode);
            newNode->setParent(node);
            updateHeights(tempRoot);
            //balanceTree(newNode);
            return newNode;
        }
        else{
            node = recursiveInsert(node->getLeft(), newNode);
        }
    }
    else{
        if(node->getRight() == NULL){
            node->setRight(newNode);
            newNode->setParent(node);
            //updateHeights(tempRoot);
            //balanceTree(newNode);
            return newNode;
        }
        else{
            node = recursiveInsert(node->getRight(), newNode);
        }
    }
    return node;
}

template<typename Key, typename Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value> *node){
    int h = 0;
    if(node != NULL){
        int lh = height(node->getLeft());
        int rh = height(node->getRight());
        int max;
        if(lh>rh)
            max = lh;
        max = rh;
        h = max + 1;
    }
    return h;
}
template<typename Key, typename Value>
int AVLTree<Key, Value>::balanceFactor(AVLNode<Key, Value> *node){
    int lh = 0;
    int rh = 0;
    if(node->getLeft() == NULL && node->getRight() == NULL){
        return 0;
    }
    //if(node->getLeft() != NULL){
        lh = height(node->getLeft());  
    //}
    //if(node->getLeft() != NULL){
        rh = height(node->getRight());  
    //}
    int bF = rh - lh;
    return bF;
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::correctHeight(AVLNode<Key, Value> *node){
    if(node->getLeft() != NULL && node->getRight() != NULL){
        int hl = node->getLeft()->getHeight();
        int hr = node->getRight()->getHeight();
        node->setHeight((hl>hr ? hl : hr)+1);   
    }
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateRightRight(AVLNode<Key, Value> *node){
    AVLNode<Key, Value> *pRoot = node;
    AVLNode<Key, Value> *rotate = node->getLeft();
    if(pRoot == this->mRoot){
        if(rotate->getRight() != NULL){
            node->setLeft(rotate->getRight());
        }
        else{
            node->setLeft(NULL);
        }
        rotate->setRight(node);
        rotate->setParent(NULL);
        node->setParent(rotate);
        this->mRoot = rotate;  
    }
    else{
        rotate->setRight(node);
        pRoot->getParent()->setLeft(rotate);
        rotate->setParent(pRoot->getParent());
        node->setParent(rotate);
        node->setParent(NULL);
    }
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateLeftLeft(AVLNode<Key, Value> *node){
    AVLNode<Key, Value> *pRoot = node;
    AVLNode<Key, Value> *rotate = node->getRight();
    if(pRoot == this->mRoot){
        if(rotate->getLeft() != NULL){
            node->setRight(rotate->getLeft());
        }
        else{
            node->setRight(NULL);
        }
        rotate->setLeft(node);
        rotate->setParent(NULL);
        node->setParent(rotate);
        this->mRoot = rotate;    
    }
    else{
        rotate->setLeft(node);
        pRoot->getParent()->setRight(rotate);
        rotate->setParent(pRoot->getParent());
        node->setParent(rotate);
        node->setRight(NULL);
    }
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateLeftRight(AVLNode<Key, Value> *node){
    rotateLeftLeft(node);
    //rotateRightRight(node);
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateRightLeft(AVLNode<Key, Value> *node){
    rotateRightRight(node->getLeft());
    //rotateLeftLeft(node);
}


template<typename Key, typename Value>
void AVLTree<Key, Value>::balanceTree(AVLNode<Key, Value> *node){
    int bF = balanceFactor(node);
    if(bF > 1){
        if(balanceFactor(node->getRight()) > 0){
            rotateLeftLeft(node);
        }
        else{
            rotateLeftRight(node);
        }
    }
    else if(bF < -1){
        if(balanceFactor(node->getLeft()) > 0){
            rotateRightLeft(node);  
        }
        else
            rotateRightRight(node);
    }
    else if(node->getParent() != NULL){
        balanceTree(node->getParent());
    }
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::updateHeights(AVLNode<Key, Value> *node){
    if(node == NULL){
        return;
    }
    updateHeights(node->getRight());
    updateHeights(node->getLeft());
    if(node->getRight() == NULL && node->getLeft() == NULL){
        node->setHeight(0);
        return;
    }
    else if(node->getLeft() == NULL){
        node->setHeight(node->getRight()->getHeight()+1);
        return;
    }
    else if(node->getRight() == NULL){
        node->setHeight(node->getLeft()->getHeight()+1);
        return;
    }
    else{
        node->setHeight(std::max(node->getLeft()->getHeight()+1,node->getRight()->getHeight()+1));
    }
}


/**
* Remove function for a given key. Finds the node, reattaches pointers, and then balances when finished. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
   // TODO
    bool found = false;
    AVLNode<Key, Value> *toDelete =  static_cast<AVLNode<Key,Value>*>(this->mRoot);
    AVLNode<Key, Value> *successor;
    while(!found){
        if(toDelete == NULL){
            break;
        }
        if(key == toDelete->getKey()){
            found = true;
        }
        else if(key < toDelete->getKey()){
            toDelete = toDelete->getLeft();
        }
        else if(key > toDelete->getKey()){
            toDelete = toDelete->getRight();
        }
    }
    if(toDelete != NULL){
        if(toDelete->getLeft() == NULL && toDelete->getRight() == NULL){
            if(toDelete->getParent()->getRight() == toDelete){
                toDelete->getParent()->setRight(NULL);
            }
            else{
                toDelete->getParent()->setLeft(NULL);
            }
            successor = toDelete->getParent();
            toDelete->setLeft(NULL);
            toDelete->setRight(NULL);
            toDelete->setParent(NULL);
        }
        else if(toDelete->getLeft() == NULL){
            if(toDelete->getParent()->getRight() == toDelete){
                toDelete->getParent()->setRight(toDelete->getRight());
            }
            else{
                toDelete->getParent()->setLeft(toDelete->getRight());
            }
            successor = toDelete->getRight();
            toDelete->getRight()->setParent(toDelete->getParent());
            toDelete->setLeft(NULL);
            toDelete->setRight(NULL);
            toDelete->setParent(NULL);
            toDelete = NULL;
        }
        else if(toDelete->getRight() == NULL){
            if(toDelete->getParent()->getRight() == toDelete){
                toDelete->getParent()->setRight(toDelete->getLeft());
            }
            else{
                toDelete->getParent()->setLeft(toDelete->getLeft());
            }
            successor = toDelete->getLeft();
            toDelete->getLeft()->setParent(toDelete->getParent());
            toDelete->setLeft(NULL);
            toDelete->setRight(NULL);
            toDelete->setParent(NULL);
            toDelete = NULL;
        }
        else if(toDelete == this->mRoot){
            successor = toDelete->getRight();
            while(successor->getLeft() != NULL){
                successor = successor->getLeft();
            }
            std::cerr<<successor->getValue();
            if(successor->getParent()->getRight() == successor){
                successor->getParent()->setRight(NULL);
            }
            else{
                successor->getParent()->setLeft(NULL);
            }
            this->mRoot = successor;
            successor->setParent(NULL);
            successor->setLeft(toDelete->getLeft());
            successor->setRight(toDelete->getRight());
        }
        else{
            successor = toDelete->getRight();
            while(successor->getLeft() != NULL){
                successor = successor->getLeft();
            }
            if(toDelete->getParent()->getRight() == toDelete){
                toDelete->getParent()->setRight(successor);
            }
            else{
                toDelete->getParent()->setLeft(successor);
            }
            successor->setLeft(toDelete->getLeft());
            successor->getParent()->setLeft(successor->getRight());
            if(toDelete->getRight() != successor){
                successor->setRight(toDelete->getRight());
            }
            else{
                successor->setRight(NULL);
            }
            toDelete->setLeft(NULL);
            toDelete->setRight(NULL);
            toDelete->setParent(NULL);
            toDelete = NULL;
        }
        updateHeights(static_cast<AVLNode<Key,Value>*>(this->mRoot));
        if(toDelete == NULL){
            balanceTree(successor);
        }
        else{
            AVLNode<Key,Value> *tmp = static_cast<AVLNode<Key,Value>*>(this->mRoot);
            while(tmp->getRight() !=NULL){
                tmp = tmp->getRight();
            }
            balanceTree(tmp);
        }
    }
}
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getRoot(){
    return static_cast<AVLNode<Key,Value>*>(this->mRoot);
}
/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif