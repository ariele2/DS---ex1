#include "d_linked_list.h"
#include "AVL_v2.h"
#include <string>
#include <memory>

using namespace std;

int main() {
    int range = 33;
    d_linked_list<AVL_v2_t<std::string>> list1;
    AVL_v2_t<std::string> tree1;
    AVL_v2_t<std::string> tree2;
    AVL_v2_t<std::string> tree3;
    AVL_v2_t<std::string> tree4;
    tree1.makeTree(range, "something");
    for (int i=0; i<range; i++) {
        tree2.insert(i, "stam");
    }
    for (int i=1; i<range*range; i+=i) {
        tree3.insert(i, "shit");
    }
    for (int i=0; i<range*6; i++) {
        tree4.insert(i, "shtok");
    }
    for (int i=1; i<range*6; i+=i) {
        tree4.remove(i);
    }
    list1.add_head(0);
    list1.get_head()->data = tree1;
    list1.add_after(2, tree2, list1.get_tail());
    list1.add_after(20, tree3, list1.get_tail());
    list1.add_after(8, tree4, (list1.get_tail()->prev));
    std::cout << "from head to tail:" <<std::endl;
    auto *tmp = list1.get_head();
    while (tmp != NULL) {
        std::cout << "node id: " << tmp->id <<std::endl;
        std::cout << " node data inorder: " <<std::endl; 
        (tmp->data).printInorder();
        std::cout << " node data preorder: " <<std::endl;
        (tmp->data).printPreorder();
        std::cout << " node data parent-order: "<<std::endl;
        (tmp->data).printReverseInorder();
        tmp = tmp->next;
    }
    list1.remove(list1.get_tail()->prev); //should delete tree4
    list1.remove(list1.get_tail()); //should remove tree3
    std::cout << "from tail to head:" <<std::endl;
    tmp = list1.get_tail();
    while (tmp != NULL) {
        std::cout << "node id: " << tmp->id <<std::endl;
        std::cout << " node data inorder: " <<std::endl; 
        (tmp->data).printInorder();
        std::cout << " node data preorder: " <<std::endl;
        (tmp->data).printPreorder();
        std::cout << " node data parent-order: "<<std::endl;
        (tmp->data).printReverseInorder();
        tmp = tmp->prev;
    }
    return 0;
}