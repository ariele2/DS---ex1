#include <memory>

template <class T>
struct Tnode {
    int id;
    int height;
    T data;
    Tnode<T> *left, *right;
};
