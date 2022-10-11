#pragma once

#include "sopasolver.hpp"

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
};
