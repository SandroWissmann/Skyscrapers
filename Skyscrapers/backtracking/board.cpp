#include "board.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

namespace backtracking {

Board::Board(int size)
    : skyscrapers{makeSkyscrapers(size)}, nopes{makeNopes(size)}
{
}

std::vector<std::vector<int>> Board::makeSkyscrapers(int size)
{
    std::vector<int> skyscraperRow(size, 0);
    return std::vector<std::vector<int>>(size, skyscraperRow);
}

std::vector<std::vector<Nopes>> Board::makeNopes(int size)
{
    std::vector<Nopes> nopesRow(size, Nopes{size - 1});
    return std::vector<std::vector<Nopes>>(size, nopesRow);
}

void debug_print(Board &board, const std::string &title)
{
    std::cout << title << '\n';
    for (std::size_t y = 0; y < board.skyscrapers.size(); ++y) {
        for (std::size_t x = 0; x < board.skyscrapers[y].size(); ++x) {

            if (board.skyscrapers[y][x] != 0) {
                std::cout << std::setw(board.skyscrapers.size() * 2);
                std::cout << "V" + std::to_string(board.skyscrapers[y][x]);
            }
            else if (board.skyscrapers[y][x] == 0 &&
                     !board.nopes[y][x].isEmpty()) {
                auto nopes_set = board.nopes[y][x].values();
                std::vector<int> nopes(nopes_set.begin(), nopes_set.end());
                std::sort(nopes.begin(), nopes.end());

                std::string nopesStr;
                for (std::size_t i = 0; i < nopes.size(); ++i) {
                    nopesStr.append(std::to_string(nopes[i]));
                    if (i != nopes.size() - 1) {
                        nopesStr.push_back(',');
                    }
                }
                std::cout << std::setw(board.skyscrapers.size() * 2);
                std::cout << nopesStr;
            }
            else {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

} // namespace permutation
