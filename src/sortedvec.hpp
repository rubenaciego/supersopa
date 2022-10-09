#pragma once

#include "sopasolver.hpp"

struct node {
  int i;
  int j;
  int iters;
  std::string word;
  std::vector<std::vector<bool> > visited;
  int left;
  int right;
};

class SortedVecSolver : public SopaSolver
{
private:
  std::vector<std::string> d;
  int max_length = -1;
  int size_d = -1;

  void search(int i0, int j0, std::list<std::string>& found, int n);

public:
    virtual void initWords(const std::list<std::string>& words) override;
    virtual void findWords(std::list<std::string>& found) override;
};
