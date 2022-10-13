#include "hashmap.hpp"
#include <cmath>

#include <iostream>

void DoubleHash::initDoubleHash(int n)
{
    collisions = 0;
    hashTableSize = 2*n;
    hashTable.resize(hashTableSize, "");
    bitsetSieveOfEratosthenes.resize(hashTableSize,true);
    sieveOfEratosthenes();

    getBiggestPrime(hashTableSize);
}

void DoubleHash::sieveOfEratosthenes()
{
    for (uint64_t pr = 2; pr * pr < hashTableSize; pr++) {
        if (bitsetSieveOfEratosthenes[pr] == true) {
            for (uint64_t i = pr * pr; i < hashTableSize; i += pr)
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
        hash1 = (hash1 * p1 + (uint64_t)s[i]) % p;

    return hash1;
}

uint64_t DoubleHash::rollingHash2(const std::string& s) const
{
    uint64_t hash2 = 0;

    for (int i = 0; i < s.length(); ++i)
        hash2 = (hash2 * p2 + (uint64_t)s[i]) % p;

    return hash2;
}

uint64_t DoubleHash::updateRollingHash1(uint64_t currhash, uint64_t next) const
{
   return (currhash * p1 + next) % p;
}

uint64_t DoubleHash::updateRollingHash2(uint64_t currhash, uint64_t next) const
{
    return (currhash * p2 + next) % p;
}

void DoubleHash::addWord(const std::string& s)
{
    uint64_t hashValue = rollingHash1(s) % hashTableSize;
    uint64_t hashOffset = toHashOffset(rollingHash2(s));

    while(hashTable[hashValue] != "") {
        ++collisions;
        hashValue = (hashValue + hashOffset)%hashTableSize;
    }

    hashTable[hashValue] = s;
}

uint64_t DoubleHash::toHashOffset(uint64_t h) const
{
    return biggestPrime - (h%biggestPrime);
}

bool DoubleHash::searchWord(const std::string& s,
    uint64_t hashValue, uint64_t hashOffset) const
{
    hashValue = hashValue % hashTableSize;
    hashOffset = toHashOffset(hashOffset);
    uint64_t initialHashValue = hashValue;
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

    uint64_t nprefixes = 0;
    for (const std::string& s : words)
    {
        nprefixes += s.length();
        maxlen = std::max(maxlen, s.length());
        minlen = std::min(minlen, s.length());
        doubleHash.addWord(s);
    }

    prefixHash.initDoubleHash(nprefixes);

    for (const std::string& s : words)
    {
        for (int i = 0; i < s.length(); ++i)
        {
            std::string pref = s.substr(0, i + 1);
            prefixHash.addWord(pref);
        }
    }

    std::cerr << "HashMap number of collisions: " << doubleHash.collisions <<
    std::endl;
}

void HashMapSolver::findWords(std::unordered_set<std::string>& found)
{
    std::vector<std::vector<bool>> seen(sopa.size(), std::vector<bool> (sopa[0].size(), false));
    std::string res;
    res.reserve(maxlen);
    lettersVisited = totalOperations = 0;

    int numOfPos = sopa.size()*sopa[0].size(), posDone = 0;
    std::cerr << "Starting search with HashMapSolver" << std::endl;
    for (int i = 0; i < sopa.size(); ++i)
    {
        for (int j = 0; j < sopa[i].size(); ++j) {
            findWordsFrom(i, j, seen, 0, 0, 0, res, found);
            ++posDone;
            std::cerr << (double)posDone*100.0/(double)numOfPos << "%" <<
            std::endl;
        }
    }
}

void HashMapSolver::findWordsFrom(int i, int j, std::vector<std::vector<bool>>& seen,
        size_t currlen, uint64_t h1, uint64_t h2, std::string& res,
        std::unordered_set<std::string>&
        found)
{
    if (seen[i][j] || currlen >= maxlen) return;
    ++lettersVisited;
    h1 = doubleHash.updateRollingHash1(h1, (uint64_t)sopa[i][j]);
    h2 = doubleHash.updateRollingHash2(h2, (uint64_t)sopa[i][j]);
    seen[i][j] = true;
    res.push_back(sopa[i][j]);

    if (!prefixHash.searchWord(res, h1, h2))
    {
        res.pop_back();
        seen[i][j] = false;
        return;
    }

    if (currlen + 1 >= minlen) {
        if (doubleHash.searchWord(res, h1, h2))
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
                findWordsFrom(ni, nj, seen, currlen + 1, h1, h2, res, found);
        }
    }

    res.pop_back();
    seen[i][j] = false;
}
