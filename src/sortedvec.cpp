#include "sortedvec.hpp"

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

void SortedVecSolver::search(int i, int j, std::unordered_set<std::string>& found, int len,
    std::string& word, int left, int right, std::vector<std::vector<bool>>& visited)
{
    if (visited[i][j] || len >= max_length || left > right) return;
    ++lettersVisited;
    visited[i][j] = true;
    word.push_back(sopa[i][j]);
    int n = sopa.size();

    left = std::lower_bound(d.begin() + left, d.begin() + right+1, word,
        [len](const std::string& l, const std::string& r) {
            return l[len] < r[len];
    }) - d.begin();

    right = std::lower_bound(d.begin() + left, d.begin() + right+1, word,
        [len](const std::string& l, const std::string& r) {
            return l[len] < r[len] + 1;
    }) - d.begin() - 1;

    totalOperations += 2;

    if ((left <= right) && (d[left] == word)) found.insert(word);

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

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            search(i, j, found, 0, res, 0, size_d - 1, visited);
    }
}
