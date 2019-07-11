#include <iostream>
#include <chrono>
#include <fstream>
#include <cmath>
#include "Heap.h"
#include "DHeap.h"

using namespace std;
using namespace std::chrono;

int TotalComparacoes = 0;

#define MAX_REPETICAO 5

function<int(int, int)> MIN_HEAP = [](int a, int b) -> int {
    TotalComparacoes++;
    return a - b;
};

string formatMemoria(unsigned long int memoria)
{
    if (memoria < 1024)
    {
        return to_string(memoria) + "b";
    }
    else if (memoria < 1048576)
    {
        return to_string(memoria / 1024 ) + "kB";
    }
    else if (memoria < 1073741824)
    {
        return to_string(memoria / 1048576 ) + "MB";
    }
    else
    {
        return to_string(memoria / 1073741824 ) + "GB";
    }
}

double media(vector<int>& results)
{
    int sum = 0;
    for (int result : results)
    {
        sum += result;
    }

    return sum / results.size();
}

double desvioPadrao(vector<int>& results, int media)
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
    vector<int> duracoes;
    unsigned long int comparacoes = 0;
    unsigned long int memoria = 0;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        TotalComparacoes = 0;

        Heap<int>* heap = new DHeap<int>(d, MIN_HEAP);

        high_resolution_clock::time_point tIni = high_resolution_clock::now();

        for (int j = quantidade; j > 0; j--)
        {
            heap->push(j);
        }

        high_resolution_clock::time_point tFim = high_resolution_clock::now();

        duracoes.push_back(duration_cast<milliseconds>( tFim - tIni ).count());

        comparacoes = TotalComparacoes;
        memoria = max(heap->memoryUsage(), memoria);

        delete heap;
    }

    cout << d << ", " << quantidade;

    double mean = media(duracoes);

    double desvio = desvioPadrao(duracoes, mean);

    for(int duracao : duracoes)
    {
        cout << ", " << duracao << "ms"; 
    }

    cout
        << ", " << round(mean) << "ms"
        << ", " << desvio
        << ", " << comparacoes
        << ", " << formatMemoria(memoria)
        << endl;
}

void TestarConstroiPiorCase(int d, int quantidade)
{
    vector<int> duracoes;
    unsigned long int comparacoes = 0;
    unsigned long int memoria = 0;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        TotalComparacoes = 0;

        Heap<int>* heap = new DHeap<int>(d, MIN_HEAP);
        vector<int> initList;

        for (int j = quantidade; j > 0; j--)
        {
            initList.push_back(j);
        }

        high_resolution_clock::time_point tIni = high_resolution_clock::now();

        heap->create(initList);

        high_resolution_clock::time_point tFim = high_resolution_clock::now();

        duracoes.push_back(duration_cast<milliseconds>( tFim - tIni ).count());

        comparacoes = TotalComparacoes;
        memoria = max(heap->memoryUsage(), memoria);

        delete heap;
    }

    cout << d << ", " << quantidade;

    double mean = media(duracoes);

    double desvio = desvioPadrao(duracoes, mean);    

    for(int duracao : duracoes)
    {
        cout << ", " << duracao << "ms"; 
    }

    cout
        << ", " << round(mean) << "ms"
        << ", " << desvio
        << ", " << comparacoes
        << ", " << formatMemoria(memoria)
        << endl;
}

int main(int argc, char *argv[])
{
    int values[] = {/*1000, 10000, 100000, 1000000, 2000000, */ 5000000};
    int ds[] = {2, 10, 20, 50, 70, 100, 120, 150, 170, 200, 220, 250, 270, 300};

    high_resolution_clock::time_point tIni = high_resolution_clock::now();

    for (int v : values)
    {
        cout << "---------------- PIOR CASO (" << v << ") ----------------" << endl;
        cout << "                 INSERIR                 " << endl;
        cout << "D, N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;

        for (int d : ds)
        {
            TestarPushPiorCase(d, v);
        }

        cout << "                 CONSTROI                 " << endl;
        cout << "D, N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;

        for (int d : ds)
        {
            TestarConstroiPiorCase(d, v);
        }
    }

    high_resolution_clock::time_point tFim = high_resolution_clock::now();

    cout << "Duracao total do experimento: " << duration_cast<seconds>( tFim - tIni ).count() << "s" << endl;

    return 0;
}
