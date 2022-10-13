#include "hashmap.hpp"
#include <cmath>

#include <iostream>

void DoubleHash::initDoubleHash(int n)
{
    hashTableSize = 1000*n;
    hashTable.resize(hashTableSize, "");
    bitsetSieveOfEratosthenes.resize(hashTableSize,true);
    sieveOfEratosthenes();

    getBiggestPrime(hashTableSize);
}

void DoubleHash::sieveOfEratosthenes()
{
    for (int p = 2; p * p <= hashTableSize; p++) {
        if (bitsetSieveOfEratosthenes[p] == true) {
            for (int i = p * p; i <= hashTableSize; i += p)
                bitsetSieveOfEratosthenes[i] = false;
        }
    }
}

void DoubleHash::getBiggestPrime(int n)
{
    biggestPrime = n-1;
    while(bitsetSieveOfEratosthenes[biggestPrime] == false)
        biggestPrime--;
}

uint64_t DoubleHash::rollingHash1(const std::string& s) const
{
    uint64_t hash1 = 0;

    for (int i = 0; i < s.length(); ++i)
        hash1 = (hash1 * p1 + (uint64_t)s[i]) % hashTableSize;

    return hash1;
}

uint64_t DoubleHash::rollingHash2(const std::string& s) const
{
    uint64_t hash2 = 0;

    for (int i = 0; i < s.length(); ++i)
        hash2 = (hash2 * p2 + (uint64_t)s[i]) % hashTableSize;

    hash2 = biggestPrime - hash2%biggestPrime;

    return hash2;
}


bool DoubleHash::searchWord(const std::string& s) const
{
    uint64_t hashValue = rollingHash1(s);
    uint64_t initialHashValue = hashValue;
    uint64_t hashOffset = rollingHash2(s);
    bool firstIteration = true;

    while(true) {
        if(hashTable[hashValue] == "")
            break;
        else if(hashTable[hashValue] == s)
            return true;
        else if(hashValue == initialHashValue && !firstIteration)
            break;
        else
            hashValue = (hashValue + hashOffset)%hashTableSize;
        firstIteration = false;
    }
    return false;
}

void HashMapSolver::initWords(const std::list<std::string>& words) {
    int n = words.size();
    doubleHash.initDoubleHash(n);

    maxlen = 0;
    minlen = std::numeric_limits<uint64_t>::max();
    int collisions = 0;

    for (const std::string& s : words)
    {
        maxlen = std::max(maxlen, s.length());
        minlen = std::min(minlen, s.length());
        uint64_t hashValue = doubleHash.rollingHash1(s);
        uint64_t hashOffset = doubleHash.rollingHash2(s);

        while(doubleHash.hashTable[hashValue] != "") {
            ++collisions;
            hashValue = (hashValue + hashOffset)%doubleHash.hashTableSize;
        }

        doubleHash.hashTable[hashValue] = s;
    }

    std::cerr << "Number of collisions: " << collisions << std::endl;
}

void HashMapSolver::findWords(std::unordered_set<std::string>& found)
{
    std::vector<std::vector<bool>> seen(sopa.size(), std::vector<bool> (sopa[0].size(), false));
    std::string res;
    res.reserve(maxlen);
    lettersVisited = totalOperations = 0;

    int numOfPos = sopa.size()*sopa[0].size(), posDone = 0;
    std::cout << "Starting search with HashMapSolver" << std::endl;
    for (int i = 0; i < sopa.size(); ++i)
    {
        for (int j = 0; j < sopa[i].size(); ++j) {
            findWordsFrom(i, j, seen, 0, res, found);
            ++posDone;
            std::cout << (double)posDone*100.0/(double)numOfPos << "%" << std::endl;
        }
    }
}

void HashMapSolver::findWordsFrom(int i, int j, std::vector<std::vector<bool>>& seen,
        size_t currlen, std::string& res, std::unordered_set<std::string>& found)
{
    if (seen[i][j] || currlen >= maxlen) return;
    ++lettersVisited;
    seen[i][j] = true;
    res.push_back(sopa[i][j]);


    if (currlen + 1 >= minlen) {
        if (doubleHash.searchWord(res))
            found.insert(res);
        ++totalOperations;
    }

    for (int di = -1; di <= 1; ++di)
    {
        for (int dj = -1; dj <= 1; ++dj)
        {
            if (di == 0 && dj == 0) continue;
            int ni = i + di;
            int nj = j + dj;

            if (ni >= 0 && ni < sopa.size() && nj >= 0 && nj < sopa[ni].size())
                findWordsFrom(ni, nj, seen, currlen + 1, res, found);
        }
    }

    res.pop_back();
    seen[i][j] = false;
}
