#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>

class Board;

namespace permutation {

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues);

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int N);

void solveBoard(Board &board, const std::vector<int> &clues);

} // namespace permutation

#endif
