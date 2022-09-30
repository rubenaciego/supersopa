#pragma once

#include <vector>
#include <list>
#include <string>
#include <random>

class SopaSolver
{
public:
    SopaSolver();

    void initSopa(int n);
    void initSopa(int n, const std::list<std::string>& words);

    void printSopa() const;

    virtual void initWords(const std::list<std::string>& words) = 0;
    virtual void findWords(std::list<std::string>& found) = 0;

protected:
    void initSopaEmpty(int n);
    bool putWord(const std::string& word);

    std::vector<std::vector<char>> sopa;

    std::random_device dev;
    std::mt19937 rng;
};
