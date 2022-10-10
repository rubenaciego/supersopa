#pragma once

#include "sopasolver.hpp"

class TrieSolver : public SopaSolver
{
public:
    virtual void initWords(const std::list<std::string>& words) override;
    virtual void findWords(std::unordered_set<std::string>& found) override;
    
private:
    // Struct to represent a node of a Ternary Search Tree
    struct TST {
        char letter;
        int isEndOfWord;
        Node* lowKid, equalKid, highKid;
    };
    
    TST* root;
    uint64_t maxLength;
    
    void initTST(int l, int r, const std::vector<std::string>& words);
    void insertWord(const std::string word);
    TST* searchNextLetter(char c, TST* node);
    
    void preorderTraversal(TST* node);
};
