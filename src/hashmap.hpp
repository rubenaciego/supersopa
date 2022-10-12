#pragma once

#include "sopasolver.hpp"

class DoubleHash
{
public:
    long int hashTableSize;
    std::vector<std::string> hashTable;

    void initDoubleHash(int n);

    uint64_t rollingHash1(const std::string& s) const;
    uint64_t rollingHash2(const std::string& s) const;
    bool searchWord(const std::string& s) const;



private:
    const uint64_t p1 = 31;
    const uint64_t p2 = 37;

    int biggestPrime;
    std::vector<bool> bitsetSieveOfEratosthenes;

    void sieveOfEratosthenes();
    void getBiggestPrime(int n);
};


class HashMapSolver : public SopaSolver
{
public:
    DoubleHash doubleHash;

    virtual void initWords(const std::list<std::string>& words) override;
    virtual void findWords(std::unordered_set<std::string>& found) override;

private:
    uint64_t maxlen;
    uint64_t minlen;

    void findWordsFrom(int i, int j, std::vector<std::vector<bool>>& seen,
        size_t currlen, std::string& res, std::unordered_set<std::string>& found);

};
