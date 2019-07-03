#ifndef __MELDABLE_HEAP_H__
#define __MELDABLE_HEAP_H__

#include "Heap.h"
#include <iostream>
#include <functional>
#include <stdexcept>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

// https://opendatastructures.org/ods-java/10_2_MeldableHeap_Randomize.html

/**
 * 
 * Min-Heap implementation
 * 
 * So: cmp(a, b) -> a-b //Min-Heap
 *     cmp(a, b) -> b-a //Max-Heap
 */
template<class T>
class MeldableHeap : public Heap<T>
{
public:
    MeldableHeap(function<int(T,T)> cmp);
    ~MeldableHeap();
    void push(T data);
    T pop();
    T top();
    int size();
    bool empty();
    void create(std::initializer_list<T> list);
    void create(const std::vector<T>& list);
    void destroy();

private:

    struct Node {
        Node* parent;
        Node* left;
        Node* right;
        T value;

        Node(T value)
        {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            this->value = value;
        }

        void destroy()
        {
            if (left != nullptr)
                left->destroy();
            if (right != nullptr)
                right->destroy();

            delete this;
        }
    };

    inline bool lowerThan(Node* x, Node* y)
    {
        return cmp(x->value, y->value) < 0;
    }

    inline bool greaterThan(Node* x, Node* y)
    {
        return cmp(x->value, y->value) >= 0;
    }

    Node* merge(Node* x, Node* y)
    {
        if (x == nullptr)
            return y;

        if (y == nullptr)
            return x;

        if (greaterThan(x, y))
            return merge(y, x);

        std::default_random_engine generator(system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> distribution(0, 1);
        
        if (distribution(generator) == 0)
        {
            x->left = merge(x->left, y);
            x->left->parent = x;
        }
        else
        {
            x->right = merge(x->right, y);
            x->right->parent = x;
        }
        
        return x;
    }

    Node* root;
    function<int(T,T)> cmp;
    int count;
};

template<class T>
MeldableHeap<T>::MeldableHeap(function<int(T,T)> cmp)
{
    this->cmp = cmp;
    count = 0;
    root = nullptr;
}


template<class T>
MeldableHeap<T>::~MeldableHeap()
{
    if (root != nullptr)
        root->destroy();
}

template<class T>
void MeldableHeap<T>::push(T data)
{
    root = merge(root, new Node(data));
    count++;
}

template<class T>
T MeldableHeap<T>::pop()
{
    if (empty())
    {
        throw out_of_range("The heap is empty");
    }

    Node* root = this->root;
    T value = root->value;
    count--;

    if (root->right != nullptr)
        root->right->parent = nullptr;
    
    if (root->left != nullptr)
        root->left->parent = nullptr;

    this->root = merge(root->right, root->left);
    delete root;
    return value;
}

template<class T>
T MeldableHeap<T>::top()
{
    if (empty())
    {
        throw out_of_range("The heap is empty");
    }

    return root->value;
}

template<class T>
int MeldableHeap<T>::size()
{
    return count;
}

template<class T>
bool MeldableHeap<T>::empty()
{
    return count == 0;
}

template<class T>
void MeldableHeap<T>::create(std::initializer_list<T> list)
{
    for(T el : list)
    {
        root = merge(root, new Node(el));
    }

    count = list.size();
}

template<class T>
void MeldableHeap<T>::create(const std::vector<T>& list)
{
    for(T el : list)
    {
        root = merge(root, new Node(el));
    }

    count = list.size();
}

template<class T>
void MeldableHeap<T>::destroy()
{
    root->destroy();
    root = nullptr;
    count = 0;
}

#endif