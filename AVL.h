#include "Tnode.h"
#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

template <class T>
class AVL_t {
    Tnode<T>* root; // private element

    int max(int num1, int num2);
    int height(Tnode<T>* t); //returns the height of the node
    Tnode<T>* findMinSubTree(Tnode<T>* root); //finds the node with the smallest id in a given subtree
    Tnode<T>* rightRotate(Tnode<T>* &to_rotate); //rotates TO the right
    Tnode<T>* leftRotate(Tnode<T>* &to_rotate); //rotates TO the left
    Tnode<T>* leftRightRotate(Tnode<T>* &to_rotate); //first rotate to the left then to the right
    Tnode<T>* rightLeftRotate(Tnode<T>* &to_rotate); //first rotate to the right then to the left
    Tnode<T>* insert(int id, T data, Tnode<T>* &root);
    Tnode<T>* remove(int id, Tnode<T>* &root);
    void inorder(Tnode<T>* root);
    void preorder(Tnode<T>* root); //for debugging purpose only!
    public:
    AVL_t<T>() { //initialize root to nullptr
        root = nullptr;
    }
    void printInorder(); //for debugging purpose only!
    void printPreorder();
    void insert(int id, T data);
    void remove(int id);
};

//inner functions of the db - not exposed to the user:

template <class T>
int AVL_t<T>::max(int num1, int num2) {
    return (num1>=num2 ? num1 : num2);
}

template <class T>
int AVL_t<T>::height(Tnode<T>* t) {
    if (!t) { 
        return -1;
    }
    return t->height;
}

template <class T>
void AVL_t<T>::inorder(Tnode<T>* root) {
    if (!root) {
        return;
    }
    inorder(root->left);
    std::cout << root->id << " ";
    inorder(root->right);
}

template <class T>
void AVL_t<T>::preorder(Tnode<T>* root) {
    if (!root) {
        return;
    }
    std::cout << root->id << " ";
    preorder(root->left);
    preorder(root->right);
}

template <class T>
Tnode<T>* AVL_t<T>::findMinSubTree(Tnode<T>* root) {//returns the minimum element inside a sub tree
    if (!root) { //safety insurance
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
Tnode<T>* AVL_t<T>::rightRotate(Tnode<T>* &to_rotate) { //rotates right, when the balance factor >= 2
    Tnode<T>* heir = to_rotate->left;
    to_rotate->left = heir->right;
    heir->right = to_rotate;
    // dont forget to update the heights...
    to_rotate->height = max(height(to_rotate->left), height(to_rotate->right))+1;
    heir->height = max(height(heir->left), height(heir->right))+1;
    return heir;
}

template <class T>
Tnode<T>* AVL_t<T>::leftRotate(Tnode<T>* &to_rotate) { //rotates left, when the balance factor <= -2
    Tnode<T>* heir = to_rotate->right;
    to_rotate->right = heir->left;
    heir->left = to_rotate;
    // dont forget to update the heights...
    to_rotate->height = max(height(to_rotate->left), height(to_rotate->right))+1;
    heir->height = max(height(heir->left), height(heir->right))+1;
    return heir;
}

template <class T>
Tnode<T>* AVL_t<T>::leftRightRotate(Tnode<T>* &to_rotate) { //left then right
    to_rotate->left = leftRotate(to_rotate->left);
    return rightRotate(to_rotate);
}

template <class T>
Tnode<T>* AVL_t<T>::rightLeftRotate(Tnode<T>* &to_rotate) { //left then right
    to_rotate->right = rightRotate(to_rotate->right);
    return leftRotate(to_rotate);
}

template <class T>
Tnode<T>* AVL_t<T>::insert(int id, T data, Tnode<T>* &root) {
    if (!root) { //if we reached a place with no such id then:
        root = new Tnode<T>; //makes all the ptrs shared ones - no memory leaks.
        root->id = id;
        root->data = data;
        root->height = 0;
        root->left = nullptr;
        root->right = nullptr;
    }
    else if (id < root->id) { //goes to the left if the id is smaller
        root->left = insert(id, data, root->left);
        if ((height(root->left)-height(root->right))>=2) {
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
        if ((height(root->left)-height(root->right))<=-2) {
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
    //update the height for all the nodes in the insertion path
    root->height = max(height(root->left),height(root->right))+1;
    return root;
}

template <class T>
Tnode<T>* AVL_t<T>::remove(int id, Tnode<T>* &root) {
    Tnode<T>* temp;
    if (!root) { //element not found!
        return nullptr;
    }
    else if (id < root->id) {
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
            root->data = temp->data; //we are using shared ptrs, no need to override=.
            //goes to the right to remove the node that we replaced.
            root->right = remove(root->id, root->right);
        }
        else { //the node that we remove has 0/1 subtree childs.
            temp = root;
            if (!(root->left)) { 
                root = root->right;
            }
            else if (!(root->right)) { 
                root = root->left;
            }
            delete temp;
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

//publice functions - for the user to use

template <class T>
void AVL_t<T>::printInorder() {
    inorder(root);
    std::cout<<std::endl;
}

template <class T>
void AVL_t<T>::printPreorder() {
    preorder(root);
    std::cout<<std::endl;
}

template <class T>
void AVL_t<T>::insert(int id, T data) {
    //insert always returns the root of the tree, and we will need to update it if necessary.
    root = insert(id, data, root); 
}

template <class T>
void AVL_t<T>::remove(int id) {
    //same as insert, need to update if necessary
    root = remove(id, root);
}
