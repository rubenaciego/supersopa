#include "bloom.hpp"
#include <cmath>
#include <limits>
#include <iostream>

IndependentHash::IndependentHash(size_t k)
{
    coefs.resize(k);

    bool nonzero = false;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, p - 1);

    do
    {
        for (int i = 0; i < k; ++i)
        {
            coefs[i] = dist(rng);
            nonzero |= i != 0 && coefs[i] != 0;
        }
    } while (!nonzero);   
}

uint64_t IndependentHash::operator()(uint64_t x) const
{
    uint64_t res = 0;

    for (int i = coefs.size() - 1; i >= 0; --i)
        res = (res * x + coefs[i]) % p;

    return res;
}

void BloomSolver::initWords(const std::list<std::string>& words)
{
    const double desiredP = 1e-16;
    uint64_t n = words.size();
    uint64_t k = (uint64_t)(-log2(desiredP));
    uint64_t m = (uint64_t)(-(double)n * log(desiredP)/(log(2)*log(2)));

    std::cerr << "Bloom filter: m = " << m << ", k = " << k << std::endl;

    hashes.reserve(k);

    for (int i = 0; i < k; ++i)
        hashes.emplace_back(k);

    bitset.resize(m);
    maxlen = 0;
    minlen = std::numeric_limits<uint64_t>::max();

    for (const std::string& s : words)
    {
        maxlen = std::max(maxlen, s.length());
        minlen = std::min(minlen, s.length());
        uint64_t hash = rollingHash(s);
        addBloom(hash);
    }
}

void BloomSolver::findWords(std::unordered_set<std::string>& found)
{
    std::vector<std::vector<bool>> seen(sopa.size(), std::vector<bool> (sopa[0].size(), false));
    std::string res;
    res.reserve(maxlen);
    lettersVisited = totalOperations = 0;

    int numOfPos = sopa.size()*sopa[0].size(), posDone = 0;
    std::cout << "Starting search with BloomSolver" << std::endl;
    for (int i = 0; i < sopa.size(); ++i)
    {
        for (int j = 0; j < sopa[i].size(); ++j)
        {
            findWordsFrom(i, j, seen, 0, 0, res, found);
            ++posDone;
            std::cout << (double)posDone*100.0/(double)numOfPos << "%" << std::endl;
        }
    }
}

void BloomSolver::findWordsFrom(int i, int j, std::vector<std::vector<bool>>& seen, uint64_t curr_hash,
        size_t currlen, std::string& res, std::unordered_set<std::string>& found)
{
    if (seen[i][j] || currlen >= maxlen) return;
    ++lettersVisited;
    seen[i][j] = true;
    curr_hash = (curr_hash * b + sopa[i][j]) % p;
    res.push_back(sopa[i][j]);

    if (currlen + 1 >= minlen && checkBloom(curr_hash))
        found.insert(res);

    for (int di = -1; di <= 1; ++di)
    {
        for (int dj = -1; dj <= 1; ++dj)
        {
            if (di == 0 && dj == 0) continue;
            int ni = i + di;
            int nj = j + dj;

            if (ni >= 0 && ni < sopa.size() && nj >= 0 && nj < sopa[ni].size())
                findWordsFrom(ni, nj, seen, curr_hash, currlen + 1, res, found);
        }
    }

    res.pop_back();
    seen[i][j] = false;
}

uint64_t BloomSolver::rollingHash(const std::string& s) const
{
    uint64_t res = 0;

    for (int i = 0; i < s.length(); ++i)
        res = (res * b + (uint64_t)s[i]) % p;
    
    return res;
}
void BloomSolver::addBloom(uint64_t val)
{
    for (int i = 0; i < hashes.size(); ++i)
        bitset[hashes[i](val) % bitset.size()] = true;
}

bool BloomSolver::checkBloom(uint64_t val)
{
    for (int i = 0; i < hashes.size(); ++i)
    {
        uint64_t index = hashes[i](val) % bitset.size();
        totalOperations += hashes.size();
        if (!bitset[index]) return false;
    }

    return true;
}
