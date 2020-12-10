#ifndef AVL_V2_H
#define AVL_V2_H

#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

template<class T>
struct Tnode_v2 {
    int id;
    int height;
    T data;
    std::shared_ptr<Tnode_v2<T>> left, right, parent;
};

template <class T>
class AVL_v2_t {
    std::shared_ptr<Tnode_v2<T>> root; 
    std::shared_ptr<Tnode_v2<T>> smallest_node; 

    int max(int num1, int num2);
    int height(std::shared_ptr<Tnode_v2<T>> t); //returns the height of the node
    std::shared_ptr<Tnode_v2<T>> findMinSubTree(std::shared_ptr<Tnode_v2<T>> root); //finds the node with the smallest id in a given subtree
    std::shared_ptr<Tnode_v2<T>> rightRotate(std::shared_ptr<Tnode_v2<T>> to_rotate); //rotates to the right
    std::shared_ptr<Tnode_v2<T>> leftRotate(std::shared_ptr<Tnode_v2<T>> to_rotate); //rotates to the left
    std::shared_ptr<Tnode_v2<T>> leftRightRotate(std::shared_ptr<Tnode_v2<T>> to_rotate); //first rotate to the left then to the right
    std::shared_ptr<Tnode_v2<T>> rightLeftRotate(std::shared_ptr<Tnode_v2<T>> to_rotate); //first rotate to the right then to the left
    std::shared_ptr<Tnode_v2<T>> insert(int id, T data, std::shared_ptr<Tnode_v2<T>> root); //insert a id-data element to the tree keeping it balanced
    std::shared_ptr<Tnode_v2<T>> remove(int id, std::shared_ptr<Tnode_v2<T>> root); //remove a id-data element by id from the tree, keeping it balanced
    std::shared_ptr<Tnode_v2<T>> makeTreeByRange(int range, int start, T data, std::shared_ptr<Tnode_v2<T>> root); //makes a tree with id 0-range.
    std::shared_ptr<Tnode_v2<T>> getNodeById(int id, std::shared_ptr<Tnode_v2<T>> root); 
    void inorder(std::shared_ptr<Tnode_v2<T>> root); //makes inorder search on the tree
    void preorder(std::shared_ptr<Tnode_v2<T>> root); //for debugging purpose only!
    void reverseInorder(std::shared_ptr<Tnode_v2<T>> smallest_node); //for debugging purpose only!
    void clearTree(std::shared_ptr<Tnode_v2<T>> root); //clears all the tree, called by the destructor.
    public:
    AVL_v2_t<T>() { //initialize root and smallest node to nullptr
        root = nullptr;
        smallest_node = nullptr;
    }
    ~AVL_v2_t<T>() { //makes all the pointers null, so shared ptr will release the data
        smallest_node = nullptr;
        clearTree(root);
    }
    void printInorder(); //for debugging purpose only!
    void printPreorder(); //for debugging purpose only!
    void printReverseInorder(); //for debugging purpose only!
    //bool find (int id);
    void insert(int id, T data);
    void remove(int id);
    void makeTree(int range, T data); //supply a range and data, it will make a new tree with keys from 0-range and data.
    std::shared_ptr<Tnode_v2<T>> getNode(int id);
    class Iterator;
    Iterator begin() const;
    Iterator end() const;
    bool isEmpty(); //return true if the tree is empty
};

template <class T>
class AVL_v2_t<T>::Iterator {
    const AVL_v2_t<T>* tree;
    std::shared_ptr<Tnode_v2<T>> curr_node;
    Iterator(const AVL_v2_t<T>* tree, std::shared_ptr<Tnode_v2<T>> curr_node): tree(tree), curr_node(curr_node){};
    friend class AVL_v2_t<T>;
    public:
    Tnode_v2<T> operator*() const;
    Iterator& operator++();
    bool operator==(const Iterator& it) const;
    bool operator!=(const Iterator& it) const;
};

template <class T>
Tnode_v2<T> AVL_v2_t<T>::Iterator::operator*() const {
    //need to check curr_node first
    return *curr_node;
}

template <class T>
typename AVL_v2_t<T>::Iterator& AVL_v2_t<T>::Iterator::operator++() {
    std::shared_ptr<Tnode_v2<T>> p;
    if (!curr_node) {
        curr_node = tree->root;
        // if curr_node is still null need an exception here
        while (curr_node->left) {
            curr_node = curr_node->left;
        }
    }
    else {
        if (curr_node->right) {
            // successor is the farthest left node of right subtree
            curr_node = curr_node->right;
            while (curr_node->left) {
                curr_node = curr_node->left;
            }
        }
        else {
            p = curr_node->parent;
            while (p && curr_node == p->right) {
                curr_node = p;
                p = p->parent;
            }
            curr_node = p;
        }
    }
    return *this;
}

template <class T>
bool AVL_v2_t<T>::Iterator::operator==(const Iterator& it) const {
    return curr_node == it.curr_node;
}

template <class T>
bool AVL_v2_t<T>::Iterator::operator!=(const Iterator& it) const {
    return !(*this == it);
}

template <class T>
typename AVL_v2_t<T>::Iterator AVL_v2_t<T>::begin() const {
    return Iterator(this, smallest_node);
}

template <class T>
typename AVL_v2_t<T>::Iterator AVL_v2_t<T>::end() const {
    return Iterator(this, nullptr);
}

//inner functions of the data base - not exposed to the user.

template <class T>
int AVL_v2_t<T>::max(int num1, int num2) {
    return (num1>=num2 ? num1 : num2);
}

template <class T>
int AVL_v2_t<T>::height(std::shared_ptr<Tnode_v2<T>> t) {
    if (!t) { 
        return -1;
    }
    return t->height;
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::makeTreeByRange(int range, int start, T data, std::shared_ptr<Tnode_v2<T>> root) {
    int mid = (start+range)/2;
    if (start > range) {
        return nullptr;
    }
    else if (!root) {
        root = std::shared_ptr<Tnode_v2<T>>(new Tnode_v2<T>());
        root->id = mid;
        root->data = data;
        root->height = 0;
        root->left = nullptr;
        root->right = nullptr;
        root->parent = nullptr;
        if (!smallest_node || smallest_node->id > root->id) { //updates smallest node if needed
            smallest_node = root;
        }
    }
    root->left = makeTreeByRange(mid-1, start, data, root->left);
    if (root->left) {
        root->left->parent = root;
    }
    root->right = makeTreeByRange(range, mid+1, data, root->right);
    if (root->right) {
        root->right->parent = root;
    }
    root->height = max(height(root->left),height(root->right))+1;
    return root;
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::getNodeById(int id, std::shared_ptr<Tnode_v2<T>> root) {
    if (!root) {
        return nullptr;
    }
    else if (root->id == id) {
        return root;
    }
    else if (id < root->id) { 
        return getNodeById(id, root->left);
    }
    else { //id > root->id
        return getNodeById(id, root->right);
    }
}

template <class T>
void AVL_v2_t<T>::inorder(std::shared_ptr<Tnode_v2<T>> root) {
    if (!root) {
        return;
    }
    inorder(root->left); //go to the left
    std::cout << root->id << " "; //do something
    inorder(root->right); //go to the right
}

template <class T>
void AVL_v2_t<T>::preorder(std::shared_ptr<Tnode_v2<T>> root) {
    if (!root) {
        return;
    }
    std::cout << root->id << " "; //do something
    preorder(root->left); //go to the left
    preorder(root->right); //go to the right
}

template <class T>
void AVL_v2_t<T>::reverseInorder(std::shared_ptr<Tnode_v2<T>> smallest_node) {
    if (!smallest_node) {
        return;
    }
    std::cout << smallest_node->id << " "; //do something (starting from the smallest node)
    inorder(smallest_node->right); //do inorder
    reverseInorder(smallest_node->parent); //go to the parent
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::findMinSubTree(std::shared_ptr<Tnode_v2<T>> root) {//returns the minimum element inside a sub tree
    if (!root) { //in case we are trying to find smallest node on non existing sub tree
        return nullptr;
    }
    else if (!(root->left)){
        return root;
    }
    else {
        return findMinSubTree(root->left);
    }
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::rightRotate(std::shared_ptr<Tnode_v2<T>> to_rotate) { //rotates right, when the balance factor >= 2
    std::shared_ptr<Tnode_v2<T>> heir = to_rotate->left; 
    to_rotate->left = heir->right; //to rotate takes the right sub tree of his heir
    if (heir->right) { //updates the parent of the right sub tree (if it was existed)
        (heir->right)->parent = to_rotate;
    }
    heir->parent = to_rotate->parent; 
    to_rotate->parent = heir;
    heir->right = to_rotate;
    // dont forget to update the heights...
    to_rotate->height = max(height(to_rotate->left), height(to_rotate->right))+1;
    heir->height = max(height(heir->left), height(heir->right))+1;
    return heir;
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::leftRotate(std::shared_ptr<Tnode_v2<T>> to_rotate) { //rotates left, when the balance factor <= -2
    std::shared_ptr<Tnode_v2<T>> heir = to_rotate->right;
    to_rotate->right = heir->left; //to rotate takes the left sub tree of his heir
    if (heir->left) { //updates the parent of the left sub tree (if it was existed)
        (heir->left)->parent = to_rotate;
    }
    heir->parent = to_rotate->parent;
    to_rotate->parent = heir;
    heir->left = to_rotate;
    // dont forget to update the heights...
    to_rotate->height = max(height(to_rotate->left), height(to_rotate->right))+1;
    heir->height = max(height(heir->left), height(heir->right))+1;
    return heir;
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::leftRightRotate(std::shared_ptr<Tnode_v2<T>> to_rotate) { //left then right
    to_rotate->left = leftRotate(to_rotate->left);
    return rightRotate(to_rotate);
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::rightLeftRotate(std::shared_ptr<Tnode_v2<T>> to_rotate) { //right then left
    to_rotate->right = rightRotate(to_rotate->right);
    return leftRotate(to_rotate);
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::insert(int id, T data, std::shared_ptr<Tnode_v2<T>> root) {
    if (!root) { //if we reached a place with no such id then:
        root = std::shared_ptr<Tnode_v2<T>>(new Tnode_v2<T>()) ; //makes all the ptrs shared ones - no memory leaks.
        root->id = id;
        root->data = data;
        root->height = 0;
        root->left = nullptr;
        root->right = nullptr;
        root->parent = nullptr;
        if (!smallest_node || smallest_node->id > root->id) { //updates smallest node if needed
            smallest_node = root;
        }
    }
    else if (id < root->id) { //goes to the left if the id is smaller
        root->left = insert(id, data, root->left);
        if ((height(root->left)-height(root->right))>=2) { //after inserting, check if balance is needed
            if (id<(root->left)->id) {
                root = rightRotate(root);
            }
            else {
                root = leftRightRotate(root);
            }
        }
    }
    else if (id > root->id) { //goes to the right if the id is bigger
        root->right = insert(id, data, root->right);
        if ((height(root->left)-height(root->right))<=-2) { //after inserting, check if balance is needed
            if (id>(root->right)->id) {
                root = leftRotate(root);
            }
            else {
                root = rightLeftRotate(root);
            }
        }
    }
    else if (id == root->id) { //stops the search if the id exists already
        return root;
    }
    //update the parent for the new node if needed
    if (root->right) {
        if (!((root->right)->parent)) { //if the root->right has no parent, the it means its our new node
            (root->right)->parent = root; //update it to its parent
        }
    }
    else if (root->left) {
        if (!((root->left)->parent)) { //same as right
            (root->left)->parent = root;
        }
    }
    
    //update the height for all the nodes in the insertion path
    root->height = max(height(root->left),height(root->right))+1;
    return root;
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::remove(int id, std::shared_ptr<Tnode_v2<T>> root) {
    std::shared_ptr<Tnode_v2<T>> temp;
    if (!root) { //element not found!
        return nullptr;
    }
    if (smallest_node->id == root->id && smallest_node->id == id) { //happens only if we have max 2 nodes in the tree...
        smallest_node = root->right;
    }
    if (id < root->id) {
        if (smallest_node->id == (root->left)->id && smallest_node->id == id) { //if we are about to delete the smallest node, update it
            if (smallest_node->right) {  
                smallest_node = findMinSubTree(smallest_node->right); //find the smallest among his right sub trees
            }
            else {
                smallest_node = root;
            }
        }
        root->left = remove(id, root->left);
    }
    else if (id > root->id) {
        root->right = remove(id, root->right);
    }
    //we found our node to remove, so:
    else {
        if (root->left && root->right) { //the node that we remove has 2 subtree childs.
            temp = findMinSubTree(root->right);
            root->id = temp->id;
            root->data = temp->data; //no need to override=, we do not want a copy!
            //goes to the right to remove the node that we replaced.
            root->right = remove(root->id, root->right);
        }
        else { //the node that we remove has 0/1 subtree childs.
            temp = root;
            if (!(root->left)) { 
                root = root->right;
                if (root) {
                    root->parent = temp->parent;
                }
            }
            else if (!(root->right)) { 
                root = root->left;
                if (root) {
                    root->parent = temp->parent;
                }
            }
            if (temp->parent) {
                if (temp->parent->left && (temp->parent)->left->id == id) {
                    temp->parent->left = nullptr;
                }
                else if (temp->parent->right && (temp->parent)->right->id == id) {
                    temp->parent->right = nullptr;
                }
            }
            temp = nullptr;
        }
    }
    if (!root) { //do nothing to the first recursive recall (cause the root value is now nullptr)
        return nullptr;
    }
    // updates the height for all the nodes in the removing path.
    root->height = max(height(root->left), height(root->right))+1;
    // rotations!!! (if needed)
    if ((height(root->left)-height(root->right)) >= 2 ) { //the left side is unbalanced
        //check wether the problem is at the left or right side
        if (height((root->left)->left) - height((root->left)->right) == 1) {
            root = rightRotate(root);
        }
        else { //case the balance factor = 0 or -1.
            root = leftRightRotate(root);
        }
    }
    else if ((height(root->left)-height(root->right)) <= -2) {
        //check wether the problem is at the left or right side
        if (height((root->right)->left) - height((root->right)->right) == -1) {
            root = leftRotate(root);
        }
        else { //case the balance factor = 0 or -1.
            root = rightLeftRotate(root);
        }
    }
    return root;
}

template <class T>
void AVL_v2_t<T>::clearTree(std::shared_ptr<Tnode_v2<T>> root) { //post order traversal
    if (!root) {
        return;
    }
    clearTree(root->left);
    clearTree(root->right);
    if (root->parent) { //if the node has a parent, disconnect it
        root->parent = nullptr;
    }
    if (root->left) {
        root->left = nullptr;
    }
    if (root->right) {
        root->right = nullptr;
    }
}

//publice functions - for the user to use

template <class T>
void AVL_v2_t<T>::printInorder() {
    inorder(root);
    std::cout<<std::endl;
}

template <class T>
void AVL_v2_t<T>::printPreorder() {
    preorder(root);
    std::cout<<std::endl;
}

template <class T>
void AVL_v2_t<T>::printReverseInorder() {
    reverseInorder(smallest_node);
    std::cout<<std::endl;
}

template <class T>
void AVL_v2_t<T>::insert(int id, T data) {
    //insert always returns the root of the tree, and we will need to update it if necessary with the new updates.
    root = insert(id, data, root); 
}

template <class T>
void AVL_v2_t<T>::remove(int id) {
    //same as insert, need to update if necessary
    root = remove(id, root);
}

template <class T>
void AVL_v2_t<T>::makeTree(int range, T data) {
    root = makeTreeByRange(range-1, 0 , data , root);
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::getNode(int id) {
    return getNodeById(id, root);
}

template <class T>
bool AVL_v2_t<T>::isEmpty() {
    if (!root) {
        return true;
    }
    return false;
}

#endif //AVL_V2_H