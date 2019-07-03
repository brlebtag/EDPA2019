#ifndef __LEFTIST_HEAP_H__
#define __LEFTIST_HEAP_H__

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
class LeftistHeap : public Heap<T>
{
public:
    LeftistHeap(function<int(T,T)> cmp);
    ~LeftistHeap();
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
        int sval;
        Node* parent;
        Node* left;
        Node* right;
        T value;

        Node(T value)
        {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            sval = 0;
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

        if (greaterThan(x, y))
        {
            cur = y;
            workspace = x;
        }

        Node* root = cur;
        Node* prev = cur;
        Node* temp;

        cur = cur->right;

        while (cur != nullptr)
        {
            if (greaterThan(cur, workspace))
            {
                temp = workspace;
                workspace = cur;
                cur = temp;
                prev->right = cur;
                cur->parent = prev;
            }

            prev = cur;
            cur = cur->right;
        }

        prev->right = workspace;
        workspace->parent = prev;

        while (prev != nullptr)
        {
            if (prev->left == nullptr)
            {
                prev->left = prev->right;
                prev->right = nullptr; 
            }
            else
            {
                if (prev->left->sval < prev->right->sval)
                {
                    temp = prev->left;
                    prev->left = prev->right;
                    prev->right = temp;
                }

                prev->sval = prev->right->sval + 1;
            }            

            prev = prev->parent;
        }        

        return root;
    }

    Node* root;
    function<int(T,T)> cmp;
    int count;
};

template<class T>
LeftistHeap<T>::LeftistHeap(function<int(T,T)> cmp)
{
    this->cmp = cmp;
    count = 0;
    root = nullptr;
}


template<class T>
LeftistHeap<T>::~LeftistHeap()
{
    if (root != nullptr)
        root->destroy();
}

template<class T>
void LeftistHeap<T>::push(T data)
{
    root = merge(root, new Node(data));
    count++;
}

template<class T>
T LeftistHeap<T>::pop()
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
T LeftistHeap<T>::top()
{
    if (empty())
    {
        throw out_of_range("The heap is empty");
    }
    
    return root->value;
}

template<class T>
int LeftistHeap<T>::size()
{
    return count;
}

template<class T>
bool LeftistHeap<T>::empty()
{
    return count == 0;
}

template<class T>
void LeftistHeap<T>::create(std::initializer_list<T> list)
{
    for(T el : list)
    {
        root = merge(root, new Node(el));
    }

    count = list.size();
}

template<class T>
void LeftistHeap<T>::create(const std::vector<T>& list)
{
    for(T el : list)
    {
        root = merge(root, new Node(el));
    }

    count = list.size();
}

template<class T>
void LeftistHeap<T>::destroy()
{
    root->destroy();
    root = nullptr;
    count = 0;
}

#endif