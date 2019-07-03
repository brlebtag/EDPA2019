#ifndef __FIBONACCI_HEAP_H__
#define __FIBONACCI_HEAP_H__

#include "Heap.h"
#include <iostream>
#include <functional>
#include <stdexcept>
#include <cmath>

using namespace std;

// https://www.growingwiththeweb.com/data-structures/fibonacci-heap/overview/
// -> https://rosettacode.org/wiki/Fibonacci_heap#C.2B.2B

/**
 * 
 * Min-Heap implementation
 * 
 * So: cmp(a, b) -> a-b //Min-Heap
 *     cmp(a, b) -> b-a //Max-Heap
 */
template<class T>
class FibonacciHeap : public Heap<T>
{
public:
    FibonacciHeap(function<int(T,T)> cmp);
    ~FibonacciHeap();
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
        Node* prev;
        Node* next;
        Node* child;
        int degree;
        bool marked;
        T value;

        Node(T value)
        {
            parent = nullptr;
            child = nullptr;
            prev = this;
            next = this;            
            this->value = value;
            degree = 0;
            marked = false;
        }

        void destroy()
        {
            deleteAll(this);
        }

        static void deleteAll(Node* node)
        {
            if(node != nullptr)
            {
                Node* cur = node;

                do
                {
                    Node* del = cur;
                    cur = cur->next;
                    deleteAll(del->child);
                    delete del;
                } while(cur != node);
            }
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

    Node* merge(Node* a, Node* b)
    {
        if ( a == nullptr) return b;
		if ( b == nullptr) return a;

		if(greaterThan(a, b))
        {
			Node* temp = a;
			a = b;
			b = temp;
		}

		Node* aNext = a->next;
		Node* bPrev = b->prev;

		a->next = b;
		b->prev = a;
		aNext->prev = bPrev;
		bPrev->next = aNext;

		return a;
    }

    void addChild(Node* parent, Node* child)
    {
        child->prev = child->next=child;
		child->parent = parent;
		parent->degree++;
		parent->child = merge(parent->child, child);
    }

    void unmarkAndUnparentAll(Node* node)
    {
        if(node == nullptr)return;

		Node* cur = node;

		do
        {
			cur->marked = false;
			cur->parent = nullptr;
			cur = cur->next;
		}
        while ( cur != node );
    }

    Node* removeMin(Node* node)
    {
        unmarkAndUnparentAll(node->child);

		if ( node->next == node )
        {
			node = node->child;
		}
        else
        {
			node->next->prev = node->prev;
			node->prev->next = node->next;
			node = merge(node->next, node->child);
		}
		
        if ( node == nullptr ) return node;

		Node* trees[64] = { nullptr };
 
		while(true)
        {
			if ( trees[node->degree] != nullptr )
            {
				Node* temp = trees[node->degree];

				if (temp == node) break;

				trees[node->degree]=nullptr;
				
                if (lowerThan(node, temp))
                {
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
					addChild(node, temp);
				}
                else
                {
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;

					if ( node->next == node)
                    {
						temp->next = temp->prev = temp;
						addChild(temp, node);
						node = temp;
					}
                    else
                    {
						node->prev->next = temp;
						node->next->prev = temp;
						temp->next = node->next;
						temp->prev = node->prev;
						addChild(temp, node);
						node = temp;
					}
				}
				continue;
			}
            else
            {
				trees[node->degree] = node;
			}
			node = node->next;
		}

		Node* min = node;

		do
        {
			if(lowerThan(node, min))
            {
                min = node;
            }

			node = node->next;
		}
        while(node != node);
		
        return min;
    }

    Node* cut(Node* heap, Node* node)
    {
        if (node->next == node)
        {
			node->parent->child = nullptr;
		}
        else
        {
			node->next->prev = node->prev;
			node->prev->next = node->next;
			node->parent->child = node->next;
		}

		node->next = node->prev = node;
		node->marked = false;

		return merge(heap, node);
    }

    Node* decreaseKey(Node* heap, Node* node, T value)
    {
        if (cmp(node->value, value) < 0) return heap;

		node->value = value;

		if (lowerThan(node, node->parent))
        {
			heap = cut(heap, node);
			
            Node* parent = node->parent;

			node->parent = nullptr;
			
            while ( parent!=nullptr && parent->marked )
            {
				heap = cut(heap, parent);
				node = parent;
				parent = node->parent;
				node->parent = nullptr;
			}

			if( parent!=nullptr && parent->parent!=nullptr )
            {
                parent->marked = true;
            }
		}

		return heap;
    }

    Node* find(Node* heap, T value)
    {
        Node* node = heap;

		if ( node ==nullptr )
            return nullptr;

		do
        {
			if ( cmp(node->value, value) == 0 ) return node;
			
            Node* ret = find(node->child, value);

			if (ret) return ret;

			node = node->next;
		}
        while( node != heap );
		
        return nullptr;     
    }

    Node* root;
    function<int(T,T)> cmp;
    int count;
};

template<class T>
FibonacciHeap<T>::FibonacciHeap(function<int(T,T)> cmp)
{
    this->cmp = cmp;
    count = 0;
    root = nullptr;
}


template<class T>
FibonacciHeap<T>::~FibonacciHeap()
{
    if (root != nullptr)
        root->destroy();
}

template<class T>
void FibonacciHeap<T>::push(T data)
{
    root = merge(root, new Node(data));

    count++;
}

template<class T>
T FibonacciHeap<T>::pop()
{
    if (empty())
    {
        throw out_of_range("The heap is empty");
    }

    T value = root->value;

    Node* oldRoot = root;

    root = removeMin(root);

    count--;

    delete oldRoot;

    return value;
}

template<class T>
T FibonacciHeap<T>::top()
{
    if (empty())
    {
        throw out_of_range("The heap is empty");
    }

    return root->value;
}

template<class T>
int FibonacciHeap<T>::size()
{
    return count;
}

template<class T>
bool FibonacciHeap<T>::empty()
{
    return count == 0;
}

template<class T>
void FibonacciHeap<T>::create(std::initializer_list<T> list)
{
    for(T el : list)
    {
        push(el);
    }
}

template<class T>
void FibonacciHeap<T>::create(const std::vector<T>& list)
{
    for(T el : list)
    {
        push(el);
    }
}

template<class T>
void FibonacciHeap<T>::destroy()
{
    root->destroy();
    root = nullptr;
    count = 0;
}

#endif