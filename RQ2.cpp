#include <iostream>
#include <chrono>
#include <fstream>
#include <cmath>
#include "Heap.h"
#include "BinaryHeap.h"
#include "SkewHeap.h"
#include "BinaryHeap.h"
#include "LeftistHeap.h"
#include "FibonacciHeap.h"
#include "MeldableHeap.h"
#include "DHeap.h"

using namespace std;
using namespace std::chrono;

int TotalComparacoes = 0;

#define MAX_REPETICAO 5

function<int(int, int)> MIN_HEAP = [](int a, int b) -> int {
    TotalComparacoes++;
    return a - b;
};

typedef function<Heap<int>*()> Builder;

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

Heap<int>* criarBinaryHeap()
{
    return new BinaryHeap<int>(MIN_HEAP);
}

Heap<int>* criarSkewHeap()
{
    return new SkewHeap<int>(MIN_HEAP);
}

Heap<int>* criarLeftistHeap()
{
    return new LeftistHeap<int>(MIN_HEAP);
}

Heap<int>* criarFibonacciHeap()
{
    return new FibonacciHeap<int>(MIN_HEAP);
}

Heap<int>* criarMeldableHeap()
{
    return new MeldableHeap<int>(MIN_HEAP);
}

Heap<int>* criar3Heap()
{
    return new DHeap<int>(3, MIN_HEAP);
}

Heap<int>* criar4Heap()
{
    return new DHeap<int>(4, MIN_HEAP);
}

Heap<int>* criar5Heap()
{
    return new DHeap<int>(5, MIN_HEAP);
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

void TestarPiorCaso(int quantidade, Builder builder)
{
    vector<int> duracoes;
    unsigned long int comparacoes = 0;
    unsigned long int memoria = 0;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        TotalComparacoes = 0;

        Heap<int>* heap = builder();

        high_resolution_clock::time_point tIni = high_resolution_clock::now();

        for (int j = quantidade; j > 0; j--)
        {
            heap->push(j);
        }

        memoria = max(heap->memoryUsage(), memoria);

        for (int j = quantidade; j > 0; j--)
        {
            heap->pop();
        }

        high_resolution_clock::time_point tFim = high_resolution_clock::now();

        duracoes.push_back(duration_cast<milliseconds>( tFim - tIni ).count());
        comparacoes = TotalComparacoes;

        delete heap;
    }

    cout << quantidade;

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

void TestarRandom(string file, Builder builder)
{
    vector<int> duracoes;
    int quantidade, j;
    unsigned long int comparacoes = 0;
    unsigned long int memoria = 0;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        TotalComparacoes = 0;

        Heap<int>* heap = builder();
        quantidade = 0;

        ifstream fNumbers(file);

        high_resolution_clock::time_point tIni = high_resolution_clock::now();

        while (fNumbers >> j)
        {
            heap->push(j);

            quantidade++;
        }

        memoria = max(heap->memoryUsage(), memoria);

        while ( !heap->empty() )
        {
            heap->pop();
        }

        high_resolution_clock::time_point tFim = high_resolution_clock::now();

        fNumbers.close();

        duracoes.push_back(duration_cast<milliseconds>( tFim - tIni ).count());

        comparacoes = TotalComparacoes;

        delete heap;
    }

    cout << quantidade;

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

void TestarRandomOperacoes(string fNum, string fOp, Builder builder)
{
    vector<int> duracoes;
    int quantidade, j;
    string op;
    unsigned long int comparacoes = 0;
    unsigned long int memoria = 0;

    for (int i = 0; i < MAX_REPETICAO; i++)
    {
        TotalComparacoes = 0;

        Heap<int>* heap = builder();
        quantidade = 0;

        ifstream fNumbers(fNum);
        ifstream fOperations(fOp);

        high_resolution_clock::time_point tIni = high_resolution_clock::now();

        while (fOperations >> op)
        {
            if (op == "I")
            {
                if (fNumbers >> j)
                {
                    quantidade++;
                    heap->push(j);
                    memoria = max(heap->memoryUsage(), memoria);
                }
            }
            else
            {
                heap->pop();               
            }
        }
        

        high_resolution_clock::time_point tFim = high_resolution_clock::now();

        fNumbers.close();
        fOperations.close();

        comparacoes = TotalComparacoes;

        duracoes.push_back(duration_cast<milliseconds>( tFim - tIni ).count());

        delete heap;
    }

    cout << quantidade;

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
    high_resolution_clock::time_point tIni = high_resolution_clock::now();

    cout << "---------------- BINARY HEAP ----------------" << endl;
    cout << "                 PIOR CASO                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarPiorCaso(1000, criarBinaryHeap);
    TestarPiorCaso(10000, criarBinaryHeap);
    TestarPiorCaso(100000, criarBinaryHeap);
    TestarPiorCaso(1000000, criarBinaryHeap);
    TestarPiorCaso(2000000, criarBinaryHeap);
    TestarPiorCaso(5000000, criarBinaryHeap);

    cout << "                 RANDOM                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandom("n1_000.txt", criarBinaryHeap);
    TestarRandom("n10_000.txt", criarBinaryHeap);
    TestarRandom("n100_000.txt", criarBinaryHeap);
    TestarRandom("n1_000_000.txt", criarBinaryHeap);
    TestarRandom("n2_000_000.txt", criarBinaryHeap);
    TestarRandom("n5_000_000.txt", criarBinaryHeap);

    cout << "                 RANDOM OPERACOES                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandomOperacoes("n1_000.txt", "op1_000.txt", criarBinaryHeap);
    TestarRandomOperacoes("n10_000.txt", "op10_000.txt", criarBinaryHeap);
    TestarRandomOperacoes("n100_000.txt", "op100_000.txt", criarBinaryHeap);
    TestarRandomOperacoes("n1_000_000.txt", "op1_000_000.txt", criarBinaryHeap);
    TestarRandomOperacoes("n2_000_000.txt", "op2_000_000.txt", criarBinaryHeap);
    TestarRandomOperacoes("n5_000_000.txt", "op5_000_000.txt", criarBinaryHeap);

    cout << "---------------- 3-HEAP ----------------" << endl;
    cout << "                 PIOR CASO                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarPiorCaso(1000, criar3Heap);
    TestarPiorCaso(10000, criar3Heap);
    TestarPiorCaso(100000, criar3Heap);
    TestarPiorCaso(1000000, criar3Heap);
    TestarPiorCaso(2000000, criar3Heap);
    TestarPiorCaso(5000000, criar3Heap);

    cout << "                 RANDOM                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandom("n1_000.txt", criar3Heap);
    TestarRandom("n10_000.txt", criar3Heap);
    TestarRandom("n100_000.txt", criar3Heap);
    TestarRandom("n1_000_000.txt", criar3Heap);
    TestarRandom("n2_000_000.txt", criar3Heap);
    TestarRandom("n5_000_000.txt", criar3Heap);

    cout << "                 RANDOM OPERACOES                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandomOperacoes("n1_000.txt", "op1_000.txt", criar3Heap);
    TestarRandomOperacoes("n10_000.txt", "op10_000.txt", criar3Heap);
    TestarRandomOperacoes("n100_000.txt", "op100_000.txt", criar3Heap);
    TestarRandomOperacoes("n1_000_000.txt", "op1_000_000.txt", criar3Heap);
    TestarRandomOperacoes("n2_000_000.txt", "op2_000_000.txt", criar3Heap);
    TestarRandomOperacoes("n5_000_000.txt", "op5_000_000.txt", criar3Heap);

    cout << "---------------- 4-HEAP ----------------" << endl;
    cout << "                 PIOR CASO                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarPiorCaso(1000, criar4Heap);
    TestarPiorCaso(10000, criar4Heap);
    TestarPiorCaso(100000, criar4Heap);
    TestarPiorCaso(1000000, criar4Heap);
    TestarPiorCaso(2000000, criar4Heap);
    TestarPiorCaso(5000000, criar4Heap);

    cout << "                 RANDOM                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandom("n1_000.txt", criar4Heap);
    TestarRandom("n10_000.txt", criar4Heap);
    TestarRandom("n100_000.txt", criar4Heap);
    TestarRandom("n1_000_000.txt", criar4Heap);
    TestarRandom("n2_000_000.txt", criar4Heap);
    TestarRandom("n5_000_000.txt", criar4Heap);

    cout << "                 RANDOM OPERACOES                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandomOperacoes("n1_000.txt", "op1_000.txt", criar4Heap);
    TestarRandomOperacoes("n10_000.txt", "op10_000.txt", criar4Heap);
    TestarRandomOperacoes("n100_000.txt", "op100_000.txt", criar4Heap);
    TestarRandomOperacoes("n1_000_000.txt", "op1_000_000.txt", criar4Heap);
    TestarRandomOperacoes("n2_000_000.txt", "op2_000_000.txt", criar4Heap);
    TestarRandomOperacoes("n5_000_000.txt", "op5_000_000.txt", criar4Heap);

    cout << "---------------- 5-HEAP ----------------" << endl;
    cout << "                 PIOR CASO                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarPiorCaso(1000, criar5Heap);
    TestarPiorCaso(10000, criar5Heap);
    TestarPiorCaso(100000, criar5Heap);
    TestarPiorCaso(1000000, criar5Heap);
    TestarPiorCaso(2000000, criar5Heap);
    TestarPiorCaso(5000000, criar5Heap);

    cout << "                 RANDOM                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandom("n1_000.txt", criar5Heap);
    TestarRandom("n10_000.txt", criar5Heap);
    TestarRandom("n100_000.txt", criar5Heap);
    TestarRandom("n1_000_000.txt", criar5Heap);
    TestarRandom("n2_000_000.txt", criar5Heap);
    TestarRandom("n5_000_000.txt", criar5Heap);

    cout << "                 RANDOM OPERACOES                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandomOperacoes("n1_000.txt", "op1_000.txt", criar5Heap);
    TestarRandomOperacoes("n10_000.txt", "op10_000.txt", criar5Heap);
    TestarRandomOperacoes("n100_000.txt", "op100_000.txt", criar5Heap);
    TestarRandomOperacoes("n1_000_000.txt", "op1_000_000.txt", criar5Heap);
    TestarRandomOperacoes("n2_000_000.txt", "op2_000_000.txt", criar5Heap);
    TestarRandomOperacoes("n5_000_000.txt", "op5_000_000.txt", criar5Heap);

    cout << "---------------- LEFTIST HEAP ----------------" << endl;
    cout << "                 PIOR CASO                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarPiorCaso(1000, criarLeftistHeap);
    TestarPiorCaso(10000, criarLeftistHeap);
    TestarPiorCaso(100000, criarLeftistHeap);
    TestarPiorCaso(1000000, criarLeftistHeap);
    TestarPiorCaso(2000000, criarLeftistHeap);
    TestarPiorCaso(5000000, criarLeftistHeap);

    cout << "                 RANDOM                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandom("n1_000.txt", criarLeftistHeap);
    TestarRandom("n10_000.txt", criarLeftistHeap);
    TestarRandom("n100_000.txt", criarLeftistHeap);
    TestarRandom("n1_000_000.txt", criarLeftistHeap);
    TestarRandom("n2_000_000.txt", criarLeftistHeap);
    TestarRandom("n5_000_000.txt", criarLeftistHeap);

    cout << "                 RANDOM OPERACOES                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandomOperacoes("n1_000.txt", "op1_000.txt", criarLeftistHeap);
    TestarRandomOperacoes("n10_000.txt", "op10_000.txt", criarLeftistHeap);
    TestarRandomOperacoes("n100_000.txt", "op100_000.txt", criarLeftistHeap);
    TestarRandomOperacoes("n1_000_000.txt", "op1_000_000.txt", criarLeftistHeap);
    TestarRandomOperacoes("n2_000_000.txt", "op2_000_000.txt", criarLeftistHeap);
    TestarRandomOperacoes("n5_000_000.txt", "op5_000_000.txt", criarLeftistHeap);

    cout << "---------------- FIBONACCI HEAP ----------------" << endl;
    cout << "                 PIOR CASO                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarPiorCaso(1000, criarFibonacciHeap);
    TestarPiorCaso(10000, criarFibonacciHeap);
    TestarPiorCaso(100000, criarFibonacciHeap);
    TestarPiorCaso(1000000, criarFibonacciHeap);
    TestarPiorCaso(2000000, criarFibonacciHeap);
    TestarPiorCaso(5000000, criarFibonacciHeap);

    cout << "                 RANDOM                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandom("n1_000.txt", criarFibonacciHeap);
    TestarRandom("n10_000.txt", criarFibonacciHeap);
    TestarRandom("n100_000.txt", criarFibonacciHeap);
    TestarRandom("n1_000_000.txt", criarFibonacciHeap);
    TestarRandom("n2_000_000.txt", criarFibonacciHeap);
    TestarRandom("n5_000_000.txt", criarFibonacciHeap);

    cout << "                 RANDOM OPERACOES                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandomOperacoes("n1_000.txt", "op1_000.txt", criarFibonacciHeap);
    TestarRandomOperacoes("n10_000.txt", "op10_000.txt", criarFibonacciHeap);
    TestarRandomOperacoes("n100_000.txt", "op100_000.txt", criarFibonacciHeap);
    TestarRandomOperacoes("n1_000_000.txt", "op1_000_000.txt", criarFibonacciHeap);
    TestarRandomOperacoes("n2_000_000.txt", "op2_000_000.txt", criarFibonacciHeap);
    TestarRandomOperacoes("n5_000_000.txt", "op5_000_000.txt", criarFibonacciHeap);

    cout << "---------------- SKEW HEAP ----------------" << endl;
    cout << "                 PIOR CASO                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarPiorCaso(1000, criarSkewHeap);
    TestarPiorCaso(10000, criarSkewHeap);
    TestarPiorCaso(100000, criarSkewHeap);
    TestarPiorCaso(1000000, criarSkewHeap);
    TestarPiorCaso(2000000, criarSkewHeap);
    TestarPiorCaso(5000000, criarSkewHeap);

    cout << "                 RANDOM                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandom("n1_000.txt", criarSkewHeap);
    TestarRandom("n10_000.txt", criarSkewHeap);
    TestarRandom("n100_000.txt", criarSkewHeap);
    TestarRandom("n1_000_000.txt", criarSkewHeap);
    TestarRandom("n2_000_000.txt", criarSkewHeap);
    TestarRandom("n5_000_000.txt", criarSkewHeap);

    cout << "                 RANDOM OPERACOES                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandomOperacoes("n1_000.txt", "op1_000.txt", criarSkewHeap);
    TestarRandomOperacoes("n10_000.txt", "op10_000.txt", criarSkewHeap);
    TestarRandomOperacoes("n100_000.txt", "op100_000.txt", criarSkewHeap);
    TestarRandomOperacoes("n1_000_000.txt", "op1_000_000.txt", criarSkewHeap);
    TestarRandomOperacoes("n2_000_000.txt", "op2_000_000.txt", criarSkewHeap);
    TestarRandomOperacoes("n5_000_000.txt", "op5_000_000.txt", criarSkewHeap);

    cout << "---------------- MELDABLE HEAP ----------------" << endl;
    cout << "                 PIOR CASO                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarPiorCaso(1000, criarMeldableHeap);
    TestarPiorCaso(10000, criarMeldableHeap);
    TestarPiorCaso(100000, criarMeldableHeap);
    TestarPiorCaso(1000000, criarMeldableHeap);
    TestarPiorCaso(2000000, criarMeldableHeap);
    TestarPiorCaso(5000000, criarMeldableHeap);

    cout << "                 RANDOM                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandom("n1_000.txt", criarMeldableHeap);
    TestarRandom("n10_000.txt", criarMeldableHeap);
    TestarRandom("n100_000.txt", criarMeldableHeap);
    TestarRandom("n1_000_000.txt", criarMeldableHeap);
    TestarRandom("n2_000_000.txt", criarMeldableHeap);
    TestarRandom("n5_000_000.txt", criarMeldableHeap);

    cout << "                 RANDOM OPERACOES                 " << endl;
    cout << "N, T1, T2, T3, T4, T5, Media, Desvio, Comparacoes, Memoria" << endl;
    TestarRandomOperacoes("n1_000.txt", "op1_000.txt", criarMeldableHeap);
    TestarRandomOperacoes("n10_000.txt", "op10_000.txt", criarMeldableHeap);
    TestarRandomOperacoes("n100_000.txt", "op100_000.txt", criarMeldableHeap);
    TestarRandomOperacoes("n1_000_000.txt", "op1_000_000.txt", criarMeldableHeap);
    TestarRandomOperacoes("n2_000_000.txt", "op2_000_000.txt", criarMeldableHeap);
    TestarRandomOperacoes("n5_000_000.txt", "op5_000_000.txt", criarMeldableHeap);

    high_resolution_clock::time_point tFim = high_resolution_clock::now();

    cout << "Duracao total do experimento: " << duration_cast<seconds>( tFim - tIni ).count() << "s" << endl;

    return 0;
}
