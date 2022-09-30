#include <iostream>

#include "sortedvec.hpp"
#include "trie.hpp"
#include "bloom.hpp"
#include "hashmap.hpp"

int main(int argc, const char* argv[])
{
    std::cout << "Choose implementation:\n\t1. Sorted vector\n\t2. Trie\n\t3. Bloom filter\n\t4. Hash map" << std::endl;
    int option;
    std::cin >> option;

    SopaSolver* solver;

    switch (option)
    {
        case 1: solver = new SortedVecSolver; break;
        case 2: solver = new TrieSolver; break;
        case 3: solver = new BloomSolver; break;
        case 4: solver = new HashMapSolver; break;
        default: std::cout << "Incorrect option" << std::endl;
    }

    return 0;
}
