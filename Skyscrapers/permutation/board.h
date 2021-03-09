#ifndef PERMUTATION_BOARD_H
#define PERMUTATION_BOARD_H

#include "nopes.h"

#include <string>
#include <vector>

namespace permutation {
struct Board {
    Board(int size);

    std::vector<std::vector<int>> skyscrapers{};
    std::vector<std::vector<Nopes>> nopes;

private:
    std::vector<std::vector<int>> makeSkyscrapers(int size);
    std::vector<std::vector<Nopes>> makeNopes(int size);
};

void debug_print(Board &board, const std::string &title = "");

} // namespace permutation

#endif