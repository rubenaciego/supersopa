#pragma once

#include "sopasolver.hpp"
#include <cstdint>

class IndependentHash
{
public:
    IndependentHash(uint64_t k);
    uint64_t operator()(uint64_t x) const;

private:
    const uint64_t p = (1LL << 61) - 1;
    std::vector<uint64_t> coefs;
};

class BloomSolver : public SopaSolver
{
public:
    BloomSolver(double colProb);
    BloomSolver(uint64_t bitfactor, uint64_t k);
    virtual void initWords(const std::list<std::string>& words) override;
    virtual void findWords(std::unordered_set<std::string>& found) override;

private:
    std::vector<IndependentHash> hashes;
    std::vector<bool> bitset;
    std::vector<bool> prefixes;
    uint64_t maxlen;
    uint64_t minlen;
    double desiredP;
    uint64_t bitfactor;
    uint64_t k;

    // Rolling hash
    const uint64_t p = (1LL << 61) - 1;
    const uint64_t b = 31;

    uint64_t rollingHash(const std::string& s) const;
    void addBloom(uint64_t val, std::vector<bool>& v);
    bool checkBloom(uint64_t val, std::vector<bool>& v);

    void findWordsFrom(int i, int j, std::vector<std::vector<bool>>& seen, uint64_t curr_hash,
        size_t currlen, std::string& res, std::unordered_set<std::string>& found);
};
