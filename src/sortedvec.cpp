#include "sortedvec.hpp"
#include <iostream>
#include <algorithm>
#include <queue>
#include <utility>

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

void SortedVecSolver::search(int i0, int j0, std::list<std::string>& found, int n) {
  std::vector<std::vector<bool> > visited(n, std::vector<bool> (n, false));
  //visited[i0][j0] = true;

  //std::cout << visited[0][0] << " " << visited[0][1] << std::endl;
  //std::cout << visited[1][0] << " " << visited[1][1] << std::endl;

  std::queue<node> q;
  node start;
  start.i = i0;
  start.j = j0;
  start.iters = 0;
  start.word = "";
  start.visited = visited;
  q.push(start);

  while (!q.empty()) {
    node cur = q.front();
    int i = cur.i;
    int j = cur.j;
    q.pop();

    cur.word += sopa[i][j];
    cur.visited[i][j] = true;
    ++cur.iters;

    if (std::binary_search(d.begin(), d.end(), cur.word)) found.push_back(cur.word);

    if (posValid(i+1,j,n) && (!cur.visited[i+1][j]) && (cur.iters <= max_length) && (cur.word <= d[size_d-1])) {
      node next = cur;
      ++next.i;
      q.push(next);
    }
    if (posValid(i,j+1,n) && (!cur.visited[i][j+1]) && (cur.iters <= max_length) && (cur.word <= d[size_d-1])) {
      node next = cur;
      ++next.j;
      q.push(next);
    }
    if (posValid(i+1,j+1,n) && (!cur.visited[i+1][j+1]) && (cur.iters <= max_length) && (cur.word <= d[size_d-1])) {
      node next = cur;
      ++next.i;
      ++next.j;
      q.push(next);
    }
    if (posValid(i-1,j,n) && (!cur.visited[i-1][j]) && (cur.iters <= max_length) && (cur.word <= d[size_d-1])) {
      node next = cur;
      --next.i;
      q.push(next);
    }
    if (posValid(i,j-1,n) && (!cur.visited[i][j-1]) && (cur.iters <= max_length) && (cur.word <= d[size_d-1])) {
      node next = cur;
      --next.j;
      q.push(next);
    }
    if (posValid(i-1,j-1,n) && (!cur.visited[i-1][j-1]) && (cur.iters <= max_length) && (cur.word <= d[size_d-1])) {
      node next = cur;
      --next.i;
      --next.j;
      q.push(next);
    }
    if (posValid(i-1,j+1,n) && (!cur.visited[i-1][j+1]) && (cur.iters <= max_length) && (cur.word <= d[size_d-1])) {
      node next = cur;
      --next.i;
      ++next.j;
      q.push(next);
    }
    if (posValid(i+1,j-1,n) && (!cur.visited[i+1][j-1]) && (cur.iters < max_length) && (cur.word <= d[size_d-1])) {
      node next = cur;
      ++next.i;
      --next.j;
      q.push(next);
    }
  }
}

void SortedVecSolver::findWords(std::list<std::string>& found) {
  int n = sopa.size();

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      search(i, j, found, n);
    }
  }
}
