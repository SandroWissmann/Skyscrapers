#ifndef CODEWARSPERMUTATION_H
#define CODEWARSPERMUTATION_H

#include <vector>

namespace codewarspermutation {

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues);

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int N);

} // namespace codewarspermutation

#endif
