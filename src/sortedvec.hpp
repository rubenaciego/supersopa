#pragma once

#include "sopasolver.hpp"
#include <iostream>
#include <algorithm>
#include <queue>
#include <utility>
#include <string>

class SortedVecSolver : public SopaSolver
{
public:
    virtual void initWords(const std::list<std::string>& words) override;
    virtual void findWords(std::unordered_set<std::string>& found) override;

private:
    std::vector<std::string> d;
    int max_length;
    int size_d;

    void search(int i, int j, std::unordered_set<std::string>& found, int len,
        std::string& word, int left, int right, std::vector<std::vector<bool>>& visited);

    bool binarySearch(const std::vector<std::string>& v, const std::string& word, int l, int r);
    int lowerBound(const std::vector<std::string>& v, const std::string& word, int l, int r, bool& found);
    int upperBound(const std::vector<std::string>& v, const std::string& word, int l, int r, int size);
};
