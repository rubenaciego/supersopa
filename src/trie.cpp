#include "trie.hpp"

void TrieSolver::initWords(const std::list<std::string>& words)
{
    std::vector<std::string> sortedWords = {words.begin(), words.end()};
    maxLength = 0;
    for (const std::string& word : sortedWords)
        maxLength = std::max(maxLength,(uint64_t) word.length());

    std::sort(sortedWords.begin(), sortedWords.end());
    root = nullptr;
    initTST(0, (int)sortedWords.size() - 1, sortedWords);
}

void TrieSolver::findWords(std::unordered_set<std::string>& found)
{
    std::vector<std::vector<bool>> seen (sopa.size(), std::vector<bool> (sopa[0].size(), false));
    std::string res;
    res.reserve(maxLength);
    lettersVisited = totalOperations = 0;

    for (int i = 0; i < sopa.size(); ++i) {
        for (int j = 0; j < sopa[i].size(); ++j)
            findWordsFrom(i, j, seen, root, 0, res, found);
    }
}

// Initializing the TST, aiming to build it as balanced as possible
void TrieSolver::initTST(int l, int r, const std::vector<std::string>& sortedWords)
{
    if (l <= r) {
        int mid = (l+r)/2;
        insertWord(sortedWords[mid], 0, &root);
        initTST(l, mid-1, sortedWords);
        initTST(mid+1, r, sortedWords);
    }
}

TST* TrieSolver::newNode(char c, int endOfWord) {
    TST* temp = new TST;
    temp->letter = c;
    temp->isEndOfWord = endOfWord;
    temp->lowKid = temp->equalKid = temp->highKid = nullptr;
    return temp;
}

//Inserting recursively the word into the trie
void TrieSolver::insertWord(const std::string& word, int i, TST** node) {
    if (*node == NULL) {
        *node = new TST();
        (*node)->letter = word[i];
        (*node)->isEndOfWord = 0;
    }
    if (word[i] < (*node)->letter)
        insertWord(word, i, &((*node)->lowKid));
    else if (word[i] > (*node)->letter)
        insertWord(word, i, &((*node)->highKid));
    else {
        if (i == (int)word.length()-1)
            (*node)->isEndOfWord = 1;
        else
            insertWord(word, i+1, &((*node)->equalKid));
    }
}

// Searching iteratively for the next letter found in the letters soup
TST* TrieSolver::searchNextLetter(char c, TST* currentNode) {
    TST* auxNode = currentNode;
    while (auxNode != nullptr) {
        ++totalOperations;
        if (c < auxNode->letter)
            auxNode = auxNode->lowKid;
        else if (c > auxNode->letter)
            auxNode = auxNode->highKid;
        else
            return auxNode;
    }
    return nullptr;
}

void TrieSolver::findWordsFrom(int i, int j, std::vector<std::vector<bool>>& seen, TST* currentNode,
                               size_t currLength, std::string& res, std::unordered_set<std::string>& found) {
    if (seen[i][j] or currLength >= maxLength) return;
    ++lettersVisited;
    TST* nextNode = searchNextLetter(sopa[i][j], currentNode);
    if (nextNode == nullptr) return;
    seen[i][j] = true;
    res.push_back(sopa[i][j]);
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
    res.pop_back();
}

void TrieSolver::preorderTraversal(TST* node) {
    if (node == nullptr) return;

    preorderTraversal(node->lowKid);
    preorderTraversal(node->equalKid);
    preorderTraversal(node->highKid);
}
