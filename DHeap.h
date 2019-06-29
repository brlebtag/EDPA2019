#ifndef __D_HEAP_H__
#define __D_HEAP_H__

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
template<unsigned int D, class T>
class DHeap : public Heap<T>
{
public:
    DHeap(function<int(T,T)> cmp);
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
        return ceil(node/((double) D)) - 1;
    }

    int first(int node)
    {
        return D * node + 1;
    }

    int last(int node)
    {
        return D * node + D;
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
            int sFirst = first(i);
            int sLast = last(i);

            for (int i = sFirst; i <= sLast; i++)
            {
                if (i <= end && greaterThan(lowerValue, i))
                {
                    lower = i;
                    lowerValue = nodes[i];
                }
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

template<unsigned int D, class T>
DHeap<D, T>::DHeap(function<int(T,T)> cmp)
{
    this->cmp = cmp;
}

template<unsigned int D, class T>
void DHeap<D, T>::push(T data)
{
    nodes.push_back(data);
    bubbleUp(nodes.size() - 1);    
}

template<unsigned int D, class T>
T DHeap<D, T>::pop()
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

template<unsigned int D, class T>
T DHeap<D, T>::top()
{
    if (empty())
    {
        throw out_of_range("The heap is empty");
    }

    return nodes[0];
}

template<unsigned int D, class T>
int DHeap<D, T>::size()
{
    return nodes.size();
}

template<unsigned int D, class T>
bool DHeap<D, T>::empty()
{
    return nodes.size() == 0;
}

template<unsigned int D, class T>
void DHeap<D, T>::create(std::initializer_list<T> list)
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

template<unsigned int D, class T>
void DHeap<D, T>::destroy()
{
    nodes.clear();
}

#endif