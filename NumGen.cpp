#include <iostream>
#include <random>
#include <functional>
#include <climits>
#include <set>
#include <string>
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
    set<int> used;
    default_random_engine generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(0, INT_MAX);
    auto dice = bind(distribution, generator);

    if (argc > 1)
    {
        int size = stoi(argv[1]);
        int rand;

        for(int i = 0; i < size; i++)
        {
            do
            {
                rand = dice();
            }
            while(used.find(rand) != used.end());

            used.insert(rand);

            cout << rand << endl;
        }
    }    

    return 0;
}
