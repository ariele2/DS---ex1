#include "d_linked_list.h"
#include "AVL_v2.h"
#include <string>
#include <memory>

using namespace std;
int main() {
    d_linked_list<AVL_v2_t<AVL_v2_t<std::string>*>> list1;
    AVL_v2_t<AVL_v2_t<std::string>*> *tree1 = new AVL_v2_t<AVL_v2_t<std::string>*>();
    //AVL_v2_t<AVL_v2_t<std::string>> tree2;
    //AVL_v2_t<AVL_v2_t<std::string>> tree3;
    //AVL_v2_t<AVL_v2_t<std::string>> tree4;
    AVL_v2_t<std::string> tree1_1;
    AVL_v2_t<std::string> tree1_2;
    tree1_1.makeTree(50, "sssss");
    tree1_2.makeTree(22, "aaaa");
    tree1_1.insert(33, "stupied");
    tree1->insert(1, &tree1_1);
    tree1->insert(2, &tree1_2);
    list1.add_head(0);
    list1.get_head()->data = *tree1;
    auto *tmp = list1.get_head();
    while (tmp != NULL) {
        std::cout << "node id: " << tmp->id <<std::endl;
        //inorder
        std::cout << "tree1 data inorder: " <<std::endl; 
        (tmp->data).printInorder();
        std::cout << "tree1_1 data inorder: " <<std::endl; 
        ((tmp->data).getNode(1))->data->printInorder();
        std::cout << "tree1_2 data inorder: " <<std::endl; 
        ((tmp->data).getNode(2))->data->printInorder();
        //preorder
        std::cout << "tree1 data preorder: " <<std::endl; 
        (tmp->data).printPreorder();
        std::cout << "tree1_1 data preorder: " <<std::endl; 
        ((tmp->data).getNode(1))->data->printPreorder();
        std::cout << "tree1_2 data preorder: " <<std::endl; 
        ((tmp->data).getNode(2))->data->printPreorder();
        //parent order
        std::cout << "tree1 data parent-order: " <<std::endl; 
        (tmp->data).printReverseInorder();
        std::cout << "tree1_1 data parent-order: " <<std::endl; 
        ((tmp->data).getNode(1))->data->printReverseInorder();
        std::cout << "tree1_2 data parent-order: " <<std::endl; 
        ((tmp->data).getNode(2))->data->printReverseInorder();
        tmp = tmp->next;
    }
    cout << "Iterator: " << endl;
    for(AVL_v2_t<std::string>::Iterator i = tree1_1.begin(); i != tree1_1.end(); ++i) {
        int num = (*i).id;
	    cout << num << endl;
    }
    delete tree1;
    return 0;
}