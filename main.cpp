#include <iostream>
#include "BinaryHeap.h"
#include "LeftistHeap.h"
#include "SkewHeap.h"
#include "DHeap.h"

int main(int argc, char *argv[])
{
    function<int(int, int)> MinHeap = [](int a, int b) -> int {
        return a - b;
    };

    function<int(int, int)> MaxHeap = [](int a, int b) -> int {
        return b - a;
    };

    Heap<int>* heap = new DHeap<4, int>(MinHeap);

    heap->push(7);

    heap->push(93);

    heap->push(32);

    heap->push(28);

    heap->push(72);

    heap->push(86);

    heap->push(54);

    heap->push(2);

    cout << heap->pop() << endl;

    cout << heap->pop() << endl;

    cout << heap->pop() << endl;

    cout << heap->pop() << endl;

    cout << heap->pop() << endl;

    cout << heap->pop() << endl;

    cout << heap->pop() << endl;

    cout << heap->pop() << endl;

    delete heap;

    return 0;
}
