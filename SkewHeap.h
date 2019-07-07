#ifndef __SKEW_HEAP_H__
#define __SKEW_HEAP_H__

#include "Heap.h"
#include <iostream>
#include <functional>
#include <stdexcept>
#include <cmath>

using namespace std;

/**
 * 
 * Min-Heap implementation
 * 
 * So: cmp(a, b) -> a-b //Min-Heap
 *     cmp(a, b) -> b-a //Max-Heap
 */
template<class T>
class SkewHeap : public Heap<T>
{
public:
    SkewHeap(function<int(T,T)> cmp);
    ~SkewHeap();
    void push(T data);
    T pop();
    T top();
    int size();
    bool empty();
    void create(std::initializer_list<T> list);
    void create(const std::vector<T>& list);
    void destroy();
    unsigned long int memoryUsage()
    {
        return sizeof(SkewHeap) + count * sizeof(Node);
    }

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

        Node* cur = x;
        Node* workspace = y;
        Node* root = cur;

        if (greaterThan(cur, workspace))
        {
            root = workspace;
        }
        
        Node* temp;

        while (cur != nullptr)
        {
            if (greaterThan(cur, workspace))
            {
                temp = cur;
                cur = workspace;
                workspace = temp;
                temp = workspace->parent;
                cur->parent = temp;
                workspace->parent = nullptr;

                if (temp != nullptr)
                {
                    temp->left = cur;
                }
            }

            temp = cur->right;
            cur->right = cur->left;
            cur->left = temp;

            if (cur->left == nullptr)
            {
                cur->left = workspace;
                workspace->parent = cur;
                break;
            }

            cur = cur->left;
        }

        return root;
    }

    Node* root;
    function<int(T,T)> cmp;
    int count;
};

template<class T>
SkewHeap<T>::SkewHeap(function<int(T,T)> cmp)
{
    this->cmp = cmp;
    count = 0;
    root = nullptr;
}


template<class T>
SkewHeap<T>::~SkewHeap()
{
    if (root != nullptr)
        root->destroy();
}

template<class T>
void SkewHeap<T>::push(T data)
{
    root = merge(root, new Node(data));
    count++;
}

template<class T>
T SkewHeap<T>::pop()
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
T SkewHeap<T>::top()
{
    if (empty())
    {
        throw out_of_range("The heap is empty");
    }

    return root->value;
}

template<class T>
int SkewHeap<T>::size()
{
    return count;
}

template<class T>
bool SkewHeap<T>::empty()
{
    return count == 0;
}

template<class T>
void SkewHeap<T>::create(std::initializer_list<T> list)
{
    for(T el : list)
    {
        root = merge(root, new Node(el));
    }

    count = list.size();
}

template<class T>
void SkewHeap<T>::create(const std::vector<T>& list)
{
    for(T el : list)
    {
        root = merge(root, new Node(el));
    }

    count = list.size();
}

template<class T>
void SkewHeap<T>::destroy()
{
    root->destroy();
    root = nullptr;
    count = 0;
}

#endif