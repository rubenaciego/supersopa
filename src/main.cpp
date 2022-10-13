#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <cassert>

#include "sortedvec.hpp"
#include "trie.hpp"
#include "bloom.hpp"
#include "hashmap.hpp"

int main(int argc, const char* argv[])
{
    if (argc != 1 && argc != 6)
    {
        std::cout << "supersopa dict_file nwords smin smax t" << std::endl;
        return 1;
    }

    const double bloomp = 1e-8;

    if (argc == 6)
    {
        int nwords = std::stoi(argv[2]);
        int minsize = std::stoi(argv[3]);
        int maxsize = std::stoi(argv[4]);
        int t = std::stoi(argv[5]);

        std::ifstream dict;
        dict.open(argv[1]);

        std::vector<std::string> words;
        std::list<std::string> wordlist;
        std::string w;

        while (dict >> w)
        {
            words.push_back(w);
            wordlist.push_back(w);
        }

        std::vector<bool> chosen(words.size());
        nwords = std::min(nwords, (int)words.size());

        std::random_device dev;
        std::mt19937 rng(dev());

        std::uniform_int_distribution<std::mt19937::result_type> sopasize(minsize, maxsize);
        std::uniform_int_distribution<std::mt19937::result_type> worddist(0, words.size() - 1);

        std::list<std::string> finalwords;

        while (nwords--)
        {
            int p;
            do
            {
                p = worddist(rng);
            } while (chosen[p]);

            chosen[p] = true;
            finalwords.push_back(words[p]);
        }

        words.clear();

        for (int i = 0; i < t; ++i)
        {
            SortedVecSolver svec;
            TrieSolver trie;
            BloomSolver bloom(bloomp);
            HashMapSolver hash;

            int n = sopasize(rng);

            svec.initSopa(n, finalwords);
            trie.initSopa(svec.getSopa());
            bloom.initSopa(svec.getSopa());
            hash.initSopa(svec.getSopa());

            svec.initWords(wordlist);
            trie.initWords(wordlist);
            bloom.initWords(wordlist);
            hash.initWords(wordlist);

            std::cout << "SuperSopa de mida " << n << std::endl;
            svec.printSopa();

            std::unordered_set<std::string> fvec, ftrie, fbloom, fhash;

            auto t1 = std::chrono::high_resolution_clock::now();
            trie.findWords(ftrie);
            auto t2 = std::chrono::high_resolution_clock::now();
            auto triedur = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

            t1 = std::chrono::high_resolution_clock::now();
            svec.findWords(fvec);
            t2 = std::chrono::high_resolution_clock::now();
            auto vecdur = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

            t1 = std::chrono::high_resolution_clock::now();
            bloom.findWords(fbloom);
            t2 = std::chrono::high_resolution_clock::now();
            auto bloomdur = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

            t1 = std::chrono::high_resolution_clock::now();
            hash.findWords(fhash);
            t2 = std::chrono::high_resolution_clock::now();
            auto hashdur = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

            std::cout << "Sorted vector: " << vecdur.count() << "ms" << std::endl
                << "Trie: " << triedur.count() << "ms" << std::endl
                << "Bloom filter: " << bloomdur.count() << "ms" << std::endl
                << "Hashmap: " << hashdur.count() << "ms" << std::endl;

            std::cout << "Sorted vector operations: visited = " << svec.getMetrics().first << ", total = " << svec.getMetrics().second << std::endl
                << "Trie operations: visited = " << trie.getMetrics().first << ", total = " << trie.getMetrics().second << std::endl
                << "Bloom filter operations: visited = " << bloom.getMetrics().first << ", total = " << bloom.getMetrics().second << std::endl
                << "Hashmap operations: visited = " << hash.getMetrics().first << ", total = " << hash.getMetrics().second << std::endl;


            std::cout << "Bloom filter false positives: " << (int)fbloom.size() - (int)fvec.size() << std::endl;
            std::cout << std::endl;

            if (fvec != ftrie)
            {
                std::cerr << "Error between sorted vector and trie implementations!" << std::endl
                    << "Present in vector and not in trie:" << std::endl;

                for (const std::string& s : fvec)
                {
                    if (!ftrie.count(s))
                        std::cerr << s << std::endl;
                }

                std::cerr << "Present in trie and not in vector:" << std::endl;

                for (const std::string& s : ftrie)
                {
                    if (!fvec.count(s))
                        std::cerr << s << std::endl;
                }
            }

            if (fvec != fbloom)
            {
                std::cerr << "Difference between sorted vector and bloom filter implementations!" << std::endl
                    << "Present in vector and not in bloom:" << std::endl;

                for (const std::string& s : fvec)
                {
                    if (!fbloom.count(s))
                        std::cerr << s << std::endl;
                }

                std::cerr << "Present in bloom and not in vector:" << std::endl;

                for (const std::string& s : fbloom)
                {
                    if (!fvec.count(s))
                        std::cerr << s << std::endl;
                }
            }
        }

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
            case 3: solver = new BloomSolver(bloomp); break;
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
            auto metrics = solver->getMetrics();

            std::cout << "Total number of letters visited: " << metrics.first << std::endl;
            std::cout << "Total number of operations: " << metrics.second << std::endl;
            std::cout << "Following words found in " << duration.count() << "ms:" << std::endl;

            for(const std::string& s : found)
                std::cout << s << std::endl;

            delete solver;
        }

        std::cout << "Choose implementation:\n\t1. Sorted vector\n\t2. Trie\n\t3. Bloom filter\n\t4. Hash map" << std::endl;
    }

    return 0;
}
