#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <vector>

namespace backtracking {

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues);

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int N);

} // namespace backtracking

#endif
