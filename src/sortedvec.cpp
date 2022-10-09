#include "sortedvec.hpp"
#include <iostream>
#include <algorithm>
#include <queue>
#include <utility>
#include <string>

void SortedVecSolver::initWords(const std::list<std::string>& words) {
  size_d = words.size();
  d = std::vector<std::string> (size_d);
  int i = 0;
  for (std::list<std::string>::const_iterator it = words.begin(); it != words.end(); ++it) {
    if ((max_length == -1) || ((*it).length() > max_length)) max_length = (*it).length();
    d[i] = *it;
    ++i;
  }

  sort(d.begin(), d.end());
  //for (int j = 0; j < size_d; ++j) std::cout << d[j] << std::endl;
  //std::cout << max_length << std::endl;
}

bool posValid(int i, int j, int n) {
  return ((i >= 0) && (i < n) && (j >= 0) && (j < n));
}

bool BinarySearch(const std::vector<std::string>& v, const std::string& word, int l, int r) {
  if (l > r) return false;

  int mid;
  mid = (l + r)/2;

  if(v[mid] == word) return true;
  else if(v[mid] > word) return BinarySearch(v, word, l, mid-1);
  else return BinarySearch(v, word, mid+1, r);
}

int lowerBound(const std::vector<std::string>& v, const std::string& word, int l, int r, bool& found) {
  if (l >= r) return r;
  int mid = (l+r)/2;
  if (v[mid] == word) found = true;
  if (v[mid] < word) return lowerBound(v, word, mid+1, r, found);
  else return lowerBound(v, word, l, mid, found);
}

int upperBound(const std::vector<std::string>& v, const std::string& word, int l, int r, int size) {
  if (l >= r) return r;
  int mid = (l+r)/2;
  if(v[mid].substr(0, size) <= word) return upperBound(v, word, mid+1, r, size);
  else return upperBound(v, word, l, mid, size);
}

void SortedVecSolver::search(int i0, int j0, std::unordered_set<std::string>& found, int n) {
  std::vector<std::vector<bool> > visited(n, std::vector<bool> (n, false));
  //visited[i0][j0] = true;

  //std::cout << visited[0][0] << " " << visited[0][1] << std::endl;
  //std::cout << visited[1][0] << " " << visited[1][1] << std::endl;

  int n_iters = 0;

  std::queue<node> q;
  node start;
  start.i = i0;
  start.j = j0;
  start.iters = 0;
  start.word = "";
  start.visited = visited;
  start.left = 0;
  start.right = d.size()-1;
  q.push(start);

  while (!q.empty()) {
    ++n_iters;
    node cur = q.front();
    int i = cur.i;
    int j = cur.j;
    q.pop();

    cur.word += sopa[i][j];
    cur.visited[i][j] = true;
    ++cur.iters;

    bool foundBound = false;

    if (cur.word > d[size_d-1]) cur.left = size_d;
    else {
      cur.left = lowerBound(d, cur.word, cur.left, cur.right, foundBound);
      cur.right = upperBound(d, cur.word, cur.left, cur.right, cur.word.length());
    }

    if (foundBound) found.insert(cur.word);
    else if (BinarySearch(d, cur.word, cur.left, cur.right)) found.insert(cur.word);

    if (posValid(i+1,j,n) && (!cur.visited[i+1][j]) && (cur.iters <= max_length) && (cur.left <= cur.right)) {
      node next = cur;
      ++next.i;
      q.push(next);
    }
    if (posValid(i,j+1,n) && (!cur.visited[i][j+1]) && (cur.iters <= max_length) && (cur.left <= cur.right)) {
      node next = cur;
      ++next.j;
      q.push(next);
    }
    if (posValid(i+1,j+1,n) && (!cur.visited[i+1][j+1]) && (cur.iters <= max_length) && (cur.left <= cur.right)) {
      node next = cur;
      ++next.i;
      ++next.j;
      q.push(next);
    }
    if (posValid(i-1,j,n) && (!cur.visited[i-1][j]) && (cur.iters <= max_length) && (cur.left <= cur.right)) {
      node next = cur;
      --next.i;
      q.push(next);
    }
    if (posValid(i,j-1,n) && (!cur.visited[i][j-1]) && (cur.iters <= max_length) && (cur.left <= cur.right)) {
      node next = cur;
      --next.j;
      q.push(next);
    }
    if (posValid(i-1,j-1,n) && (!cur.visited[i-1][j-1]) && (cur.iters <= max_length) && (cur.left <= cur.right)) {
      node next = cur;
      --next.i;
      --next.j;
      q.push(next);
    }
    if (posValid(i-1,j+1,n) && (!cur.visited[i-1][j+1]) && (cur.iters <= max_length) && (cur.left <= cur.right)) {
      node next = cur;
      --next.i;
      ++next.j;
      q.push(next);
    }
    if (posValid(i+1,j-1,n) && (!cur.visited[i+1][j-1]) && (cur.iters < max_length) && (cur.left <= cur.right)) {
      node next = cur;
      ++next.i;
      --next.j;
      q.push(next);
    }
  }
}

void SortedVecSolver::findWords(std::unordered_set<std::string>& found) {
  int n = sopa.size();
  int total_iters = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      search(i, j, found, n);
    }
  }
}
