#ifndef __HEAP_H__
#define __HEAP_H__

#include <initializer_list>
#include <vector>

template<class T>
class Heap
{
public:
    virtual void push(T data) = 0;
    virtual T pop() = 0;
    virtual T top() = 0;
    virtual int size() = 0;
    virtual bool empty() = 0;
    virtual void create(std::initializer_list<T> list) = 0;
    virtual void create(const std::vector<T>& list) = 0;
    virtual void destroy() = 0;
};

#endif