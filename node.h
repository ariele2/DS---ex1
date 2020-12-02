template <class T>
struct node {
    int id;
    T data;
    node<T> *next, *prev;
};
