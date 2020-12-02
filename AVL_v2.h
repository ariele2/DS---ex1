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
    std::shared_ptr<Tnode_v2<T>> rightRotate(std::shared_ptr<Tnode_v2<T>> to_rotate); //rotates TO the right
    std::shared_ptr<Tnode_v2<T>> leftRotate(std::shared_ptr<Tnode_v2<T>> to_rotate); //rotates TO the left
    std::shared_ptr<Tnode_v2<T>> leftRightRotate(std::shared_ptr<Tnode_v2<T>> to_rotate); //first rotate to the left then to the right
    std::shared_ptr<Tnode_v2<T>> rightLeftRotate(std::shared_ptr<Tnode_v2<T>> to_rotate); //first rotate to the right then to the left
    std::shared_ptr<Tnode_v2<T>> insert(int id, T data, std::shared_ptr<Tnode_v2<T>> root);
    std::shared_ptr<Tnode_v2<T>> remove(int id, std::shared_ptr<Tnode_v2<T>> root);
    void inorder(std::shared_ptr<Tnode_v2<T>> root);
    void preorder(std::shared_ptr<Tnode_v2<T>> root); //for debugging purpose only!
    void reverseInorder(std::shared_ptr<Tnode_v2<T>> smallest_node);
    void clearTree(std::shared_ptr<Tnode_v2<T>> root);
    public:
    AVL_v2_t<T>() { //initialize root to nullptr
        root = nullptr;
        smallest_node = nullptr;
    }
    /*~AVL_v2_t<T>() {
        clearTree(root);
    }*/
    void printInorder(); //for debugging purpose only!
    void printPreorder();
    void printReverseInorder();
    void insert(int id, T data);
    void remove(int id);
};

//inner functions of the db - not exposed to the user:

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
void AVL_v2_t<T>::inorder(std::shared_ptr<Tnode_v2<T>> root) {
    if (!root) {
        return;
    }
    inorder(root->left);
    std::cout << root->id << " ";
    inorder(root->right);
}

template <class T>
void AVL_v2_t<T>::preorder(std::shared_ptr<Tnode_v2<T>> root) {
    if (!root) {
        return;
    }
    std::cout << root->id << " ";
    preorder(root->left);
    preorder(root->right);
}

template <class T>
void AVL_v2_t<T>::reverseInorder(std::shared_ptr<Tnode_v2<T>> smallest_node) {
    if (!smallest_node) {
        return;
    }
    std::cout << smallest_node->id << " ";
    inorder(smallest_node->right);
    reverseInorder(smallest_node->parent);
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::findMinSubTree(std::shared_ptr<Tnode_v2<T>> root) {//returns the minimum element inside a sub tree
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
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::rightRotate(std::shared_ptr<Tnode_v2<T>> to_rotate) { //rotates right, when the balance factor >= 2
    std::shared_ptr<Tnode_v2<T>> heir = to_rotate->left;
    to_rotate->left = heir->right;
    if (heir->right) {
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
    to_rotate->right = heir->left;
    if (heir->left) {
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
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::rightLeftRotate(std::shared_ptr<Tnode_v2<T>> to_rotate) { //left then right
    to_rotate->right = rightRotate(to_rotate->right);
    return leftRotate(to_rotate);
}

template <class T>
std::shared_ptr<Tnode_v2<T>> AVL_v2_t<T>::insert(int id, T data, std::shared_ptr<Tnode_v2<T>> root) {
    if (!root) { //if we reached a place with no such id then:
        root = std::shared_ptr<Tnode_v2<T>>(new Tnode_v2<T>()) ;//makes all the ptrs shared ones - no memory leaks.
        root->id = id;
        root->data = data;
        root->height = 0;
        root->left = nullptr;
        root->right = nullptr;
        root->parent = nullptr;
        if (!smallest_node || smallest_node->id > root->id) {
            smallest_node = root;
        }
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
    //update the parent for the fresh node if needed
    if (root->right) {
        if (!((root->right)->parent)) {
            (root->right)->parent = root;
        }
    }
    else if (root->left) {
        if (!((root->left)->parent)) {
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
        if (root->right) {
            smallest_node = root->right;
        }
    }
    else if (id < root->id) {
        if (smallest_node->id == (root->left)->id && smallest_node->id == id) { //if we are about to delete the smallest node
            if (smallest_node->right) {
                smallest_node = findMinSubTree(smallest_node->right);
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
            if (temp->parent->left && (temp->parent)->left->id == id) {
                temp->parent->left = nullptr;
            }
            else if (temp->parent->right && (temp->parent)->right->id == id) {
                temp->parent->right = nullptr;
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

/*template <class T>
void AVL_v2_t<T>::clearTree(std::shared_ptr<Tnode_v2<T>> root) {
    if (!root) {
        return;
    }
    clearTree(root->left);
    clearTree(root->right);
    delete root;
    root = nullptr;
}*/

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
    //insert always returns the root of the tree, and we will need to update it if necessary.
    root = insert(id, data, root); 
}

template <class T>
void AVL_v2_t<T>::remove(int id) {
    //same as insert, need to update if necessary
    root = remove(id, root);
}

