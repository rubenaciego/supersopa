#pragma once

#include "sopasolver.hpp"
#include <algorithm>
#include <iostream>

// Struct to represent a node of a Ternary Search Tree
struct TST {
    char letter;
    int isEndOfWord;
    TST *lowKid, *equalKid, *highKid;
};

class TrieSolver : public SopaSolver
{
public:
    virtual void initWords(const std::list<std::string>& words) override;
    virtual void findWords(std::unordered_set<std::string>& found) override;
    
private:
    
    TST* root;
    uint64_t maxLength;
    
    void initTST(int l, int r, const std::vector<std::string>& words);
    TST* newNode(char c, int endOfWord);
    void insertWord(TST** node, const std::string& word);
    //void insertWord(const std::string word, int i, TST** node);
    TST* searchNextLetter(char c, TST* node);
    
    void findWordsFrom(int i, int j, std::vector<std::vector<bool>>& seen, TST* currentNode, 
                       size_t currLength, std::string& res, std::unordered_set<std::string>& found);
    
    void preorderTraversal(TST* node);
};
