#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <vector>

class Board;

namespace backtracking {

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues);

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int N);

void solveBoard(Board &board, const std::vector<int> &clues);

} // namespace backtracking

#endif
