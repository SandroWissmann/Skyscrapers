#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>

namespace permutation {

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues);

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int N);

} // namespace permutation

#endif
