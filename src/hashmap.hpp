#pragma once

#include "sopasolver.hpp"

class DoubleHash
{
public:
    size_t hashTableSize;
    std::vector<std::string> hashTable;
    int collisions;

    void initDoubleHash(int n);

    uint64_t rollingHash1(const std::string& s) const;
    uint64_t rollingHash2(const std::string& s) const;
    uint64_t updateRollingHash1(uint64_t currhash, uint64_t next) const;
    uint64_t updateRollingHash2(uint64_t currhash, uint64_t next) const;

    void addWord(const std::string& s);
    bool searchWord(const std::string& s, uint64_t h1, uint64_t h2) const;

private:
    const uint64_t p1 = 31;
    const uint64_t p2 = 37;
    const uint64_t p = (1LL << 61) - 1;

    int64_t biggestPrime;
    std::vector<bool> bitsetSieveOfEratosthenes;

    void sieveOfEratosthenes();
    void getBiggestPrime(int n);
    uint64_t toHashOffset(uint64_t h) const;
};


class HashMapSolver : public SopaSolver
{
public:
    DoubleHash doubleHash;
    DoubleHash prefixHash;

    virtual void initWords(const std::list<std::string>& words) override;
    virtual void findWords(std::unordered_set<std::string>& found) override;

private:
    uint64_t maxlen;
    uint64_t minlen;

    void findWordsFrom(int i, int j, std::vector<std::vector<bool>>& seen, size_t currlen,
        uint64_t h1, uint64_t h2, std::string& res,
        std::unordered_set<std::string>& found);
};
