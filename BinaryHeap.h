#ifndef __BINARY_HEAP_H__
#define __BINARY_HEAP_H__

#include "Heap.h"
#include <iostream>
#include <functional>
#include <stdexcept>
#include <vector>
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
class BinaryHeap : public Heap<T>
{
public:
    BinaryHeap(function<int(T,T)> cmp);
    void push(T data);
    T pop();
    T top();
    int size();
    bool empty();
    void create(std::initializer_list<T> list);
    void destroy();

    // void debug()
    // {
    //     cout << "----------------------------" << endl;
    //     for (int i = 0; i < nodes.size(); i++)
    //     {
    //         cout << nodes[i] << endl;
    //     }
    //     cout << "----------------------------" << endl;
    // }

private:
    vector<T> nodes;
    function<int(T,T)> cmp;

    int parent(int node)
    {
        return ceil(node/2.0) - 1;
    }

    int left(int node)
    {
        return 2 * node + 1;
    }

    int right(int node)
    {
        return 2 * node + 2;
    }
    
    void swap(int i, int j)
    {
        T el = nodes[j];
        nodes[j] = nodes[i];
        nodes[i] = el;
    }

    void bubbleUp(int i)
    {
        T value = nodes[i];
        int father = parent(i);

        // i != 0
        while (i >= 0 && lowerThan(value, father))
        {
            nodes[i] = nodes[father];
            i = father;
            father = parent(i);
        }

        nodes[i] = value;
    }
    
    inline bool lowerThan(T value, int i)
    {
        return cmp(value, nodes[i]) < 0;
    }

    inline bool greaterThan(T value, int i)
    {
        return cmp(value, nodes[i]) >= 0;
    }

    void bubbleDown(int i)
    {
        T value = nodes[i];
        int end = nodes.size() - 1;
        int lower = i;
        T lowerValue = value;

        while(i < end) 
        {
            int sLeft = left(i);
            int sRight = right(i);

            // cout<<"i : "<< i << ", end: " << end << ", lowerI: "<< lower << ", lowerValue: " << lowerValue << ", left:" << sLeft << ", right: "<< sRight << endl;

            if (sLeft <= end && greaterThan(lowerValue, sLeft))
            {
                lower = sLeft;
                lowerValue = nodes[sLeft];
            }

            if (sRight <= end && greaterThan(lowerValue, sRight))
            {
                lower = sRight;
                lowerValue = nodes[sRight];
            }

            if (lower != i)
            {
                nodes[i] = lowerValue;
                i = lower;
                lowerValue = value; // reset lowerValue to value
                continue;
            }

            break;
        }

        nodes[i] = value;
    }
};

template<class T>
BinaryHeap<T>::BinaryHeap(function<int(T,T)> cmp)
{
    this->cmp = cmp;
}

template<class T>
void BinaryHeap<T>::push(T data)
{
    nodes.push_back(data);
    bubbleUp(nodes.size() - 1);    
}

template<class T>
T BinaryHeap<T>::pop()
{
    if (empty())
    {
        throw out_of_range("The heap is empty");
    }

    T result = nodes[0];

    swap(0, nodes.size() - 1);

    nodes.pop_back();

    bubbleDown(0);

    return result;
}

template<class T>
T BinaryHeap<T>::top()
{
    if (empty())
    {
        throw out_of_range("The heap is empty");
    }

    return nodes[0];
}

template<class T>
int BinaryHeap<T>::size()
{
    return nodes.size();
}

template<class T>
bool BinaryHeap<T>::empty()
{
    return nodes.size() == 0;
}

template<class T>
void BinaryHeap<T>::create(std::initializer_list<T> list)
{
    for(auto const& el : list)
    {
        nodes.push_back(el);
    }

    int end = floor((size() - 1) / 2);

    for (int i = end; i >= 0; i--)
    {
        // cout<< "i: " << i << ", end: " << end << endl;
        // debug();
        bubbleDown(i);
    }
    
    // debug();
}

template<class T>
void BinaryHeap<T>::destroy()
{
    nodes.clear();
}

#endif