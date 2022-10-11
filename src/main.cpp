#include <iostream>
#include <chrono>

#include "sortedvec.hpp"
#include "trie.hpp"
#include "bloom.hpp"
#include "hashmap.hpp"

std::list<std::string> readDictionary() {
    std::list<std::string> l;
    std::string s;
    while (std::cin >> s) l.push_back(s);
    return l;
}

void usage()
{
    std::cout << "supersopa dict_file nwords smin smax t" << std::endl;
}

int main(int argc, const char* argv[])
{
    if (argc != 1 && argc != 6)
    {
        usage();
        return 1;
    }

    if (argc == 6)
    {


        return 0;
    }

    std::cout << "SuperSopa size: ";
    int size;
    std::cin >> size;

    std::cout << "Word count: ";
    int nword;
    std::cin >> nword;

    std::list<std::string> words;

    if (nword == 0)
    {
        std::cout << "Using default words..." << std::endl;
        words = {"hello", "test", "ruben", "pifa", "jofre", "mariona"};
    }

    for (int i = 0; i < nword; ++i)
    {
        std::string w;
        std::cin >> w;
        words.push_back(w);
    }

    std::cout << "Choose implementation:\n\t1. Sorted vector\n\t2. Trie\n\t3. Bloom filter\n\t4. Hash map" << std::endl;
    int option;

    while (std::cin >> option)
    {
        SopaSolver* solver = nullptr;

        switch (option)
        {
            case 1: solver = new SortedVecSolver; break;
            case 2: solver = new TrieSolver; break;
            case 3: solver = new BloomSolver; break;
            case 4: solver = new HashMapSolver; break;
            default: std::cout << "Incorrect option" << std::endl;
        }

        if (solver != nullptr)
        {
            solver->initSopa(size, words);
            solver->printSopa();
            std::cout << std::endl;

            solver->initWords(words);

            std::unordered_set<std::string> found;
            auto t1 = std::chrono::high_resolution_clock::now();
            solver->findWords(found);
            auto t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

            std::cout << "Following words found in " << duration.count() << "ms:" << std::endl;

            for(const std::string& s : found)
            {
                for (int i = 0; i < s.length(); ++i)
                {
                    std::cout << (s[i] == 0 ? ' ' : s[i]);
                }
                std::cout << ';' << std::endl;
            }

            delete solver;
        }

        std::cout << "Choose implementation:\n\t1. Sorted vector\n\t2. Trie\n\t3. Bloom filter\n\t4. Hash map" << std::endl;
    }

    return 0;
}
