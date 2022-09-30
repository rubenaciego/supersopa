#include <iostream>
#include "sopasolver.hpp"

SopaSolver::SopaSolver() : rng(dev())
{
}

void SopaSolver::initSopaEmpty(int n)
{
    sopa.resize(n);

    for (int i = 0; i < n; ++i)
        sopa[i].resize(n);
}

void SopaSolver::initSopa(int n)
{
    initSopaEmpty(n);

    std::uniform_int_distribution<std::mt19937::result_type> distChar('A', 'Z');

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            sopa[i][j] = distChar(rng);
    }
}

void SopaSolver::initSopa(int n, const std::list<std::string>& words)
{
    initSopaEmpty(n);

    for (const std::string& s : words)
    {
        int times = 10;
        while (!putWord(s) && times--);
    }

    std::uniform_int_distribution<std::mt19937::result_type> distChar('A', 'Z');

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (sopa[i][j] == 0)
                sopa[i][j] = distChar(rng);
        }
    }
}

bool SopaSolver::putWord(const std::string& word)
{
    int n = sopa.size();
    std::uniform_int_distribution<std::mt19937::result_type> distPos(0, n - 1);
    std::uniform_int_distribution<std::mt19937::result_type> distDir(0, 7);

    int r = distPos(rng);
    int c = distPos(rng);

    if (sopa[r][c] != 0) return false;

    std::list<std::pair<int, int>> used;

    int pos = 0;
    while (pos != word.length())
    {
        used.push_back({r, c});
        sopa[r][c] = word[pos];
        int nextdir = distDir(rng);
        bool found = false;

        for (int i = 0; i < 7; ++i)
        {
            /* Directions:
              012
              3 4
              567
            */

            int nr = r, nc = c;

            if (nextdir < 3) --nr;
            else if (nextdir > 4) ++nr;

            if (nextdir ==  0 || nextdir == 3 || nextdir == 5) --nc;
            else if (nextdir == 2 || nextdir == 4 || nextdir == 7) ++nc;

            if (nr >= 0 && nr < n && nc >= 0 && nc < n && sopa[nr][nc] == 0)
            {
                found = true;
                r = nr;
                c = nc;
                break;
            }

            nextdir = (nextdir + 1) % 8;
        }

        if (!found)
        {
            for (const std::pair<int, int>& p : used)
                sopa[p.first][p.second] = 0;

            return false;
        }

        ++pos;
    }

    return true;
}

void SopaSolver::printSopa() const
{
    for (int i = 0; i < sopa.size(); ++i)
    {
        for (int j = 0; j < sopa[i].size(); ++j)
            std::cout << sopa[i][j];

        std::cout << std::endl;
    }
}
