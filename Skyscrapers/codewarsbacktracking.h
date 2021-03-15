#ifndef CODEWARSBACKTRACKING_H
#define CODEWARSBACKTRACKING_H

#include <vector>

namespace codewarsbacktracking {

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues);

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int N);

} // namespace codewarsbacktracking

#endif
