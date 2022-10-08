#include <iostream>
#include <chrono>

#include "sortedvec.hpp"
#include "trie.hpp"
#include "bloom.hpp"
#include "hashmap.hpp"

int main(int argc, const char* argv[])
{
    std::cout << "Choose implementation:\n\t1. Sorted vector\n\t2. Trie\n\t3. Bloom filter\n\t4. Hash map" << std::endl;
    int option;
    std::cin >> option;

    SopaSolver* solver = nullptr;

    switch (option)
    {
        case 1: solver = new SortedVecSolver; break;
        case 2: solver = new TrieSolver; break;
        case 3: solver = new BloomSolver; break;
        case 4: solver = new HashMapSolver; break;
        default: std::cout << "Incorrect option" << std::endl;
    }

    solver->initSopa(5, {"HELLO", "TEST", "RUBEN", "PIFA", "JOFRE", "MARIONA"});
    solver->printSopa();

    std::list<std::string> found;
    auto t1 = std::chrono::high_resolution_clock::now();
    solver->findWords(found);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(t2 - t1);

    std::cout << "Following words found in " << duration.count() << "ms:" << std::endl;

    for(std::list<std::string>::const_iterator it = found.begin(); it != found.end(); ++it) std::cout << *it << std::endl;

    //if (solver != nullptr) delete solver;

    return 0;
}
