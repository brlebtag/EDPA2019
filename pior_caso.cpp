#include <iostream>
#include "BinaryHeap.h"
#include "LeftistHeap.h"
#include "SkewHeap.h"
#include "DHeap.h"
#include "FibonacciHeap.h"
#include "MeldableHeap.h"
#include <chrono>
#include <fstream>

using namespace std;

using namespace std::chrono;

#define MAX 1000

int main(int argc, char *argv[])
{
    unsigned long long elapsed = 0;
    unsigned long long elapsedPush = 0;
    unsigned long long elapsedPop = 0;

    function<int(int, int)> MinHeap = [](int a, int b) -> int {
        return a - b;
    };

    // function<int(int, int)> MaxHeap = [](int a, int b) -> int {
    //     return b - a;
    // };

    Heap<int>* heap = new FibonacciHeap<int>(MinHeap);


    int totalOperacoes = 0;
    int totalInserir = 0;
    int totalRemover = 0;

    high_resolution_clock::time_point tIni = high_resolution_clock::now();

    for(int i = MAX; i > 0; i--)
    {
        totalInserir++;
        totalOperacoes++;

        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        heap->push(i);

        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();

        elapsed += duration;
        elapsedPush += duration;
    }

    while(!heap->empty())
    {
        totalRemover++;
        totalOperacoes++;

        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        heap->pop();

        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();

        elapsed += duration;
        elapsedPop += duration;
    }

    high_resolution_clock::time_point tFim = high_resolution_clock::now();

    auto dTotal = duration_cast<milliseconds>( tFim - tIni ).count();

    cout << "Duracao Total: " << dTotal << "ms" << endl;
    cout << "Total bytes: " << totalInserir * (sizeof(int) * 2 + sizeof(void*) * 3 )<< "bytes" << endl;
    cout << "Total Operacoes: " << totalOperacoes << endl;
    cout << "Total push: " << totalInserir << endl;
    cout << "Total pop: " << totalRemover << endl;
    cout << "Duracao total das operacoes de insercao: " << elapsedPush  << "ns" << endl;
    cout << "Duracao média das operacoes de insercao: " << elapsedPush / totalOperacoes << "ns" << endl;
    cout << "Duracao total das operacoes de remocao: " << elapsedPop  << "ns" << endl;
    cout << "Duracao média das operacoes de remocao: " << elapsedPop / totalOperacoes << "ns" << endl;
    cout << "Duracao total das operacoes: " << elapsed  << "ns" << endl;
    cout << "Duracao média das operacoes: " << elapsed / totalOperacoes << "ns" << endl;

    return 0;
}
