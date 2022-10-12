#pragma once

#include <vector>
#include <list>
#include <string>
#include <random>
#include <unordered_set>

class SopaSolver
{
public:
    SopaSolver();

    void initSopa(int n);
    void initSopa(int n, const std::list<std::string>& words);
    void initSopa(const std::vector<std::vector<char>>& sopa);

    void printSopa() const;

    std::vector<std::vector<char>> getSopa() const;

    virtual void initWords(const std::list<std::string>& words) = 0;
    virtual void findWords(std::unordered_set<std::string>& found) = 0;
    std::pair<uint64_t, uint64_t> getMetrics();

protected:
    void initSopaEmpty(int n);
    bool putWord(const std::string& word);
    uint64_t lettersVisited;
    uint64_t totalOperations;

    std::vector<std::vector<char>> sopa;

    std::random_device dev;
    std::mt19937 rng;
};
