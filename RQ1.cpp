#include <iostream>
#include <chrono>
#include <fstream>
#include <cmath>
#include "Heap.h"
#include "BinaryHeap.h"

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


void TestarPushPiorCase(int quantidade)
{
    vector<int> results;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        Heap<int>* heap = new BinaryHeap<int>(MIN_HEAP);

        high_resolution_clock::time_point tIni = high_resolution_clock::now();

        for (int j = quantidade; j > 0; j--)
        {
            heap->push(j);
        }

        high_resolution_clock::time_point tFim = high_resolution_clock::now();

        results.push_back(duration_cast<milliseconds>( tFim - tIni ).count());

        delete heap;
    }

    cout << quantidade;

    double mean = media(results);

    double desvio = desvioPadrao(results, mean);    

    for (int result : results)
    {
        cout << ", " << result << "ms"; 
    }

    cout << ", " << round(mean) << "ms" << ", " << desvio;

    cout << endl;
}

void TestarConstroiPiorCase(int quantidade)
{
    vector<int> results;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        Heap<int>* heap = new BinaryHeap<int>(MIN_HEAP);
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

    cout << quantidade;

    double mean = media(results);

    double desvio = desvioPadrao(results, mean);    

    for (int result : results)
    {
        cout << ", " << result << "ms"; 
    }

    cout << ", " << round(mean) << "ms" << ", " << desvio;

    cout << endl;
}

void TestarPushRandom(string file)
{
    vector<int> results;

    int quantidade, j;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        quantidade = 0;

        Heap<int>* heap = new BinaryHeap<int>(MIN_HEAP);

        ifstream fNumbers(file);

        high_resolution_clock::time_point tIni = high_resolution_clock::now();

        while (fNumbers >> j)
        {
            heap->push(j);

            quantidade++;
        }

        high_resolution_clock::time_point tFim = high_resolution_clock::now();

        fNumbers.close();

        results.push_back(duration_cast<milliseconds>( tFim - tIni ).count());

        delete heap;
    }

    cout << quantidade;

    double mean = media(results);

    double desvio = desvioPadrao(results, mean);    

    for (int result : results)
    {
        cout << ", " << result << "ms"; 
    }

    cout << ", " << round(mean) << "ms" << ", " << desvio;

    cout << endl;
}

void TestarConstroiRandom(string file)
{
    vector<int> results;
    int quantidade, j;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        Heap<int>* heap = new BinaryHeap<int>(MIN_HEAP);
        vector<int> initList;
        quantidade = 0;

        ifstream fNumbers(file);

        while (fNumbers >> j)
        {
            initList.push_back(j);

            quantidade++;
        }

        fNumbers.close();

        high_resolution_clock::time_point tIni = high_resolution_clock::now();

        heap->create(initList);

        high_resolution_clock::time_point tFim = high_resolution_clock::now();

        results.push_back(duration_cast<milliseconds>( tFim - tIni ).count());

        delete heap;
    }

    cout << quantidade;

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
    cout << "---------------- INSERT : PIOR CASO ----------------" << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio" << endl;
    TestarPushPiorCase(1000);
    TestarPushPiorCase(10000);
    TestarPushPiorCase(100000);
    TestarPushPiorCase(1000000);
    TestarPushPiorCase(2000000);
    TestarPushPiorCase(5000000);

    cout << "---------------- CONSTROI : PIOR CASO ----------------" << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio" << endl;
    TestarConstroiPiorCase(1000);
    TestarConstroiPiorCase(10000);
    TestarConstroiPiorCase(100000);
    TestarConstroiPiorCase(1000000);
    TestarConstroiPiorCase(2000000);
    TestarConstroiPiorCase(5000000);

    cout << "---------------- INSERT : ALEATORIO ----------------" << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio" << endl;
    TestarPushRandom("n1_000.txt");
    TestarPushRandom("n10_000.txt");
    TestarPushRandom("n100_000.txt");
    TestarPushRandom("n1_000_000.txt");
    TestarPushRandom("n2_000_000.txt");
    TestarPushRandom("n5_000_000.txt");

    cout << "---------------- CONSTROI : ALEATORIO ----------------" << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio" << endl;
    TestarConstroiRandom("n1_000.txt");
    TestarConstroiRandom("n10_000.txt");
    TestarConstroiRandom("n100_000.txt");
    TestarConstroiRandom("n1_000_000.txt");
    TestarConstroiRandom("n2_000_000.txt");
    TestarConstroiRandom("n5_000_000.txt");

    return 0;
}
