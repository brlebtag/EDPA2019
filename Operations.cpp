#include <iostream>
#include <cstdlib>
#include <functional>
#include <climits>
#include <set>
#include <string>
#include <ctime>
#include <cmath>

using namespace std;

int random(int min = 0, int max = INT_MAX)
{
    return rand() % max + min;
}

void print_op(const char * op, int qtd)
{
    for(int i = 0; i < qtd; i++)
    {
        cout << op << endl;
    }
}

int main(int argc, char *argv[])
{
    srand (time(NULL));

    int missingOperations = 0;

    int rando;

    if (argc > 1)
    {
        int size = stoi(argv[1]);
        
        int MAX_OP = max((int) floor(size * 0.05), size);

        while( size > 0 )
        {
            rando = (random(0, MAX_OP) % (size + 1));

            print_op("I", rando);

            missingOperations += rando;
            size -= rando;

            rando = random(0, missingOperations);

            print_op("R", rando);

            missingOperations -= rando;
        }

        print_op("R", missingOperations);

    }

    return 0;
}
