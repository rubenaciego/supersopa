#pragma once

#include "sopasolver.hpp"
#include <cstdint>

class IndependentHash
{
public:
    IndependentHash(uint64_t k);
    uint64_t operator()(uint64_t x) const noexcept;

private:
    const uint64_t p = (1LL << 61) - 1;
    std::vector<uint64_t> coefs;
};

class BloomSolver : public SopaSolver
{
public:
    virtual void initWords(const std::list<std::string>& words) override;
    virtual void findWords(std::unordered_set<std::string>& found) override;

private:
    std::vector<IndependentHash> hashes;
    std::vector<bool> bitset;
    uint64_t maxlen;

    // Rolling hash
    const uint64_t p = (1LL << 61) - 1;
    const uint64_t b = 31;

    uint64_t rollingHash(const std::string& s) const;
    void addBloom(uint64_t val);
    bool checkBloom(uint64_t val) const;

    void findWordsFrom(int i, int j, std::vector<std::vector<bool>>& seen, uint64_t curr_hash,
        size_t currlen, std::string& res, std::unordered_set<std::string>& found);
};
