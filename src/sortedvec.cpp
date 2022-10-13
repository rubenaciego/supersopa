#include "sortedvec.hpp"
#include <iostream>
#include <algorithm>
#include <queue>
#include <utility>
#include <string>

void SortedVecSolver::initWords(const std::list<std::string>& words) {
    size_d = words.size();
    d = { words.begin(), words.end() };
    max_length = 0;
    
    for (const std::string& s : d)
        max_length = std::max(max_length, (int)s.length());

    sort(d.begin(), d.end());
}

bool posValid(int i, int j, int n) {
    return ((i >= 0) && (i < n) && (j >= 0) && (j < n));
}

bool SortedVecSolver::binarySearch(const std::vector<std::string>& v, const std::string& word, int l, int r) {
    if (l > r) return false;

    ++totalOperations;
    int mid = (l + r)/2;

    if(v[mid] == word) return true;
    else if(v[mid] > word) return binarySearch(v, word, l, mid-1);
    else return binarySearch(v, word, mid+1, r);
}

int SortedVecSolver::lowerBound(const std::vector<std::string>& v, const std::string& word, int l, int r, bool& found) {
    if (l >= r) return r;
    ++totalOperations;
    int mid = (l+r)/2;
    if (v[mid] == word) found = true;
    if (v[mid] < word) return lowerBound(v, word, mid+1, r, found);
    else return lowerBound(v, word, l, mid, found);
}

int SortedVecSolver::upperBound(const std::vector<std::string>& v, const std::string& word, int l, int r, int size) {
    if (l >= r) return r;
    ++totalOperations;
    int mid = (l+r)/2;
    if(v[mid].substr(0, size) <= word) return upperBound(v, word, mid+1, r, size);
    else return upperBound(v, word, l, mid, size);
}

void SortedVecSolver::search(int i, int j, std::unordered_set<std::string>& found, int len,
    std::string& word, int left, int right, std::vector<std::vector<bool>>& visited)
{
    if (visited[i][j] || len >= max_length || left > right) return;
    ++lettersVisited;
    visited[i][j] = true;
    word.push_back(sopa[i][j]);
    int n = sopa.size();
    
    bool foundBound = false;

    if (word > d[size_d-1]) left = size_d;
    else {
        left = lowerBound(d, word, left, right, foundBound);
        right = upperBound(d, word, left, right, len);
    }

    if (foundBound || binarySearch(d, word, left, right)) found.insert(word);

    for (int di = -1; di <= 1; ++di)
    {
        for (int dj = -1; dj <= 1; ++dj)
        {
            if (di == 0 && dj == 0) continue;
            if (posValid(i + di, j + dj, n))
                search(i + di, j + dj, found, len + 1, word, left, right, visited);
        }
    }

    visited[i][j] = false;
    word.pop_back();
}

void SortedVecSolver::findWords(std::unordered_set<std::string>& found) {
    int n = sopa.size();
    int total_iters = 0;
    lettersVisited = totalOperations = 0;

    std::vector<std::vector<bool>> visited(n, std::vector<bool> (n, false));
    std::string res;
    res.reserve(max_length);
    
    int numOfPos = n*n, posDone = 0;
    std::cout << "Starting search with SortedVecSolver" << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            search(i, j, found, 0, res, 0, size_d - 1, visited);
            ++posDone;
            std::cout << (double)posDone*100.0/(double)numOfPos << "%" << std::endl;
        }
    }
}
