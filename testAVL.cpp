#include "AVL.h"
#include <string>

using namespace std;

int main() {
    AVL_t<std::string> tree1;
    int range = 25;
    for (int i=0; i<range; i++) {
        tree1.insert(i, "data");
    }
    cout<< "Inorder: ";
    tree1.printInorder();
    cout<< "Preorder: ";
    tree1.printPreorder();
    for (int i=1; i<range; i+=i) {
        tree1.remove(i);
        cout << "removing... " << i <<endl;
    }
    cout<< "Inorder: ";
    tree1.printInorder();
    cout<< "Preorder: ";
    tree1.printPreorder();
    return 0; 
}