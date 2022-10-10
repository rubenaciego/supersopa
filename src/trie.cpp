#include "trie.hpp"

void TrieSolver::initWords(const std::list<std::string>& words)
{
    std::vector<std::string> sortedWords((int)words.size());
    int i = 0;
    for (const std::string& word : words) {
        sortedWords[i] = word;
        maxLength = std::max(maxLength, word.length());
        ++i;
    }
    
    std::sort(sortedWords.begin(), sortedWords.end());
    root = NULL;
    initTST(0, (int)sortedWords.size() - 1, sortedWords);
}

void TrieSolver::findWords(std::unordered_set<std::string>& found)
{

}

// Initializing the TST, aiming to build it as balanced as possible
void TrieSolver::initTST(int l, int r, const std::vector<std::string> sortedWords)
{
        if (l <= r) {
            int mid = (l+r)/2;
            insertWord(sortedWords[mid]);
            initTST(l, mid-1, sortedWords);
            initTST(mid+1, r, sortedWords);
        }
}
/*
void TrieSolver::insertWord(std::string word, int i, TST* node) {
    if (node == NULL) {
        node = new TST();
        node->letter = word[i];
        node->isEndOfWord = 0;
    }
    if (word[i] < node->letter)
        insertWord(word, i, node->lowKid);
    else if (word[i] > node->letter)
        insertWord(word, i, node->highKid);
    else {
        if (i == (int)word.length()-1)
            node->isEndofWord = 1;
        else
            insertWord(word, i+1, node->equalKid);
    }
}*/

// Insert iteratively a certain word
void TrieSolver::insertWord(std::string word) {
    TST* auxNode = root;
    int i = 0;
    while (i < (int)word.length()) {
        if (auxNode == NULL) {
            auxNode = new TST();
            auxNode->letter = word[i];
            auxNode->isEndOfWord = 0;
        }
        if (word[i] < auxNode->letter)
            auxNode = auxNode->lowKid;
        else if (word[i] > auxNode->letter)
            auxNode = auxNode->highKid;
        else {
            if (i == (int)word.length()-1)
                auxNode->isEndOfWord = 1;
            else
                auxNode = auxNode->equalKid;
            ++i;
        }
    }
}

// Searching iteratively for the next letter found in the letters soup
TST* TrieSolver::searchNextLetter(char c, TST* currentNode) {
    TST* auxNode = currentNode;
    while (auxNode != NULL) {
        if (c < auxNode->letter)
            auxNode = auxNode->lowKid;
        else if (c > auxNode->letter)
            auxNode = auxNode->highKid;
        else
            return auxNode;
    }
    
    return NULL;
}

void TrieSolver::preorderTraversal(TST* node) {
    if (node == NULL)
        return;
    
    cout << node->letter << " ";
    preorderTraversal(node->lowKid);
    preorderTraversal(node->equalKid);
    preorderTraversal(node->highKid);
}
