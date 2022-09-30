#pragma once

#include "sopasolver.hpp"

class TrieSolver : public SopaSolver
{
public:
    virtual void initWords(const std::list<std::string>& words) override;
    virtual void findWords(std::list<std::string>& found) override;
};
