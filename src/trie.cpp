#include "trie.hpp"

void TrieSolver::initWords(const std::list<std::string>& words)
{
    std::vector<std::string> sortedWords = {words.begin(), words.end()};
    maxLength = 0;
    for (const std::string& word : sortedWords)
        maxLength = std::max(maxLength, word.length());
    
    std::sort(sortedWords.begin(), sortedWords.end());
    root = NULL;
    initTST(0, (int)sortedWords.size() - 1, sortedWords);
}

void TrieSolver::findWords(std::unordered_set<std::string>& found)
{
    std::vector<std::vector<bool>> seen (sopa.size(), std::vector<bool> (sopa[0].size(), false));
    std::string res(maxLength, '\0');
    lettersVisited = totalOperations = 0;
    
    for (int i = 0; i < sopa.size(); ++i) {
        for (int j = 0; j < sopa[i].size(); ++j) {
            
            findWordsFrom(i, j, seen, root, 0, res, found);
        }
    }
}

// Initializing the TST, aiming to build it as balanced as possible
void TrieSolver::initTST(int l, int r, const std::vector<std::string>& sortedWords)
{
        if (l <= r) {
            int mid = (l+r)/2;
            insertWord(&root, sortedWords[mid]);
            initTST(l, mid-1, sortedWords);
            initTST(mid+1, r, sortedWords);
        }
}

TST* TrieSolver::newNode(char c, int endOfWord) {
    TST* temp = new TST;
    temp->letter = c;
    temp->isEndOfWord = endOfWord;
    temp->lowKid = temp->equalKid = temp->highKid = NULL;
    return temp;
}

// Insert iteratively a certain word
void TrieSolver::insertWord(TST** node, const std::string& word) {
    int i = 0;
    while (i < (int)word.length()) {
        if (*node == NULL) {
            *node = newNode(word[i], 0);
        }
        if (i+1 == (int)word.length()) {
            (*node)->isEndOfWord = 1;
            return;
        }
        
        if (word[i] < (*node)->letter)
            node = &((*node)->lowKid);
        else if (word[i] > (*node)->letter)
            node = &((*node)->highKid);
        else {
            node = &((*node)->equalKid);
            ++i;
        }
    }
}

// Searching iteratively for the next letter found in the letters soup
TST* TrieSolver::searchNextLetter(char c, TST* currentNode) {
    TST* auxNode = currentNode;
    while (auxNode != NULL) {
        ++totalOperations;
        if (c < auxNode->letter)
            auxNode = auxNode->lowKid;
        else if (c > auxNode->letter)
            auxNode = auxNode->highKid;
        else
            return auxNode;
    }
    return NULL;
}

void TrieSolver::findWordsFrom(int i, int j, std::vector<std::vector<bool>>& seen, TST* currentNode,
                               size_t currLength, std::string& res, std::unordered_set<std::string>& found) {
    ++lettersVisited;
    if (seen[i][j] or currLength >= maxLength) return;
    TST* nextNode = searchNextLetter(sopa[i][j], currentNode);
    if (nextNode == NULL) return;
    seen[i][j] = true;
    res[currLength] = sopa[i][j];
    if (nextNode->isEndOfWord)
        found.insert(res);
    
    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 and dj == 0) continue;
            int i2 = i+di;
            int j2 = j+dj;
            
            if (i2 >= 0 and i2 < sopa.size() and j2 >= 0 and j2 < sopa[i2].size())
                findWordsFrom(i2, j2, seen, nextNode->equalKid, currLength+1, res, found);
        }
    }
    
    seen[i][j] = false;
    res[currLength] = '\0';
}

void TrieSolver::preorderTraversal(TST* node) {
    if (node == NULL) return;
    
    preorderTraversal(node->lowKid);
    preorderTraversal(node->equalKid);
    preorderTraversal(node->highKid);
}
