#ifndef D_LINKED_LIST_H
#define D_LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "node.h"

template <class T>
class d_linked_list {
    node<T> *head, *tail; 
    void destroyList(node<T> *head);
    public:
    d_linked_list<T>() { //constructor sets head and tail to NULL at creation.
        head = NULL; //first element in the list
        tail = NULL; //last element in the list
    }
    ~d_linked_list() { //assume that T cleans after itself.
        destroyList(head);  
    } 
    void add_head(int id); //only works when the list is empty - the head element shouldnt be changed.
    void add_after(int id, T &data, node<T> *last_n);
    void remove(node<T> *to_remove);
    node<T>* get_tail() const;
    node<T>* get_head() const;
    class NullArgument{};
};

template <class T>
void d_linked_list<T>::destroyList(node<T> *head) {
    while (head) {
        node<T> *next = head->next;
        delete head;
        head = next;
    }
}

template <class T>
void d_linked_list<T>::add_head(int id) {
    if (!head) { //the list has no head, means its empty atm - head is perm, no shared_ptr needed.
        head = new node<T>;
        head->id = id;
		head->next = NULL;
        head->prev = NULL;
		tail = head;
    }
}

template <class T>
void d_linked_list<T>::add_after(int id, T &data, node<T> *last_n) {
    if (!last_n) {
        throw NullArgument();
    }
    node<T> *new_node = new node<T>;
    new_node->id = id;
    new_node->data = data;
    // connecting new_node and his neighbours to it.
    new_node->next = last_n->next;
    new_node->prev = last_n;
    last_n->next = new_node;
    if (tail == last_n) {
        tail = new_node;
    }
    else {
        (new_node->next)->prev = new_node;
    }
}

template <class T>
void d_linked_list<T>::remove(node<T> *to_remove) {
    if (to_remove && to_remove != head) { //safety insurance, cant remove head!
        if (to_remove == tail) {
            (to_remove->prev)->next = NULL;
            tail = to_remove->prev;
            delete to_remove;
        }
        else {
            (to_remove->prev)->next = to_remove->next;
            (to_remove->next)->prev = to_remove->prev;
            delete to_remove;
        }
    }
}

template <class T>
node<T>* d_linked_list<T>::get_tail() const {
    return tail;
}

template <class T>
node<T>* d_linked_list<T>::get_head() const {
    return head;
}

#endif // D_LINKED_LIST_H