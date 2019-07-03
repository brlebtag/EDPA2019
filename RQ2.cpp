#include <iostream>
#include <chrono>
#include <fstream>
#include <cmath>
#include "Heap.h"
#include "DHeap.h"

using namespace std;
using namespace std::chrono;

#define MAX_REPETICAO 5

function<int(int, int)> MIN_HEAP = [](int a, int b) -> int {
    return a - b;
};

double media(vector<int> results)
{
    int sum = 0;
    for (int result : results)
    {
        sum += result;
    }

    return sum / results.size();
}

double desvioPadrao(vector<int> results, int media)
{
    int sum = 0;

    for (int result : results)
    {
        sum += pow(result - media, 2);
    }

    return sqrt(sum / results.size());
}


void TestarPushPiorCase(int d, int quantidade)
{
    vector<int> results;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        Heap<int>* heap = new DHeap<int>(d, MIN_HEAP);

        high_resolution_clock::time_point tIni = high_resolution_clock::now();

        for (int j = quantidade; j > 0; j--)
        {
            heap->push(j);
        }

        high_resolution_clock::time_point tFim = high_resolution_clock::now();

        results.push_back(duration_cast<milliseconds>( tFim - tIni ).count());

        delete heap;
    }

    cout << d << ", " << quantidade;

    double mean = media(results);

    double desvio = desvioPadrao(results, mean);    

    for (int result : results)
    {
        cout << ", " << result << "ms"; 
    }

    cout << ", " << round(mean) << "ms" << ", " << desvio;

    cout << endl;
}

void TestarConstroiPiorCase(int d, int quantidade)
{
    vector<int> results;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        Heap<int>* heap = new DHeap<int>(d, MIN_HEAP);
        vector<int> initList;

        for (int j = quantidade; j > 0; j--)
        {
            initList.push_back(j);
        }

        high_resolution_clock::time_point tIni = high_resolution_clock::now();

        heap->create(initList);

        high_resolution_clock::time_point tFim = high_resolution_clock::now();

        results.push_back(duration_cast<milliseconds>( tFim - tIni ).count());

        delete heap;
    }

    cout << d << ", " << quantidade;

    double mean = media(results);

    double desvio = desvioPadrao(results, mean);    

    for (int result : results)
    {
        cout << ", " << result << "ms"; 
    }

    cout << ", " << round(mean) << "ms" << ", " << desvio;

    cout << endl;
}

int main(int argc, char *argv[])
{
    int values[] = {1000, 10000, 100000, 1000000, 2000000, 5000000};
    int ds[] = {2, 10, 20, 50, 70, 100, 120, 150, 170, 200};

    for (int v : values)
    {
        cout << "---------------- PIOR CASO (" << v << ") ----------------" << endl;
        cout << "                 INSERIR                 " << endl;
        cout << "D, N, T1, T2, T3, T4, T5, Media, Desvio" << endl;

        for (int d : ds)
        {
            TestarPushPiorCase(d, v);
        }

        cout << "                 CONSTROI                 " << endl;
        cout << "D, N, T1, T2, T3, T4, T5, Media, Desvio" << endl;

        for (int d : ds)
        {
            TestarConstroiPiorCase(d, v);
        }
    }


    return 0;
}
