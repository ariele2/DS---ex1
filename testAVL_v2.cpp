#include "AVL_v2.h"
#include <string>

using namespace std;

int main() {
    AVL_v2_t<std::string> tree1;
    int range = 5;
    for (int i=range-1; i>=0; i--) {
        tree1.insert(i, "data");
    }
    cout<< "Inorder: ";
    tree1.printInorder();
    cout<< "Preorder: ";
    tree1.printPreorder();
    cout<< "Parent Inorder: ";
    tree1.printReverseInorder();
    for (int i=2; i<range; i+=i) {
        tree1.remove(i);
        cout << "removing... " << i <<endl;
    }
    cout<< "Inorder: ";
    tree1.printInorder();
    cout<< "Preorder: ";
    tree1.printPreorder();
    cout<< "Parent Inorder: ";
    tree1.printReverseInorder();
    return 0; 
}