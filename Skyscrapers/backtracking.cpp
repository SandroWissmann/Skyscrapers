#include "backtracking.h"

#include "backtracking/board.h"
#include "backtracking/cluehints.h"
#include "backtracking/nopes.h"

#include <cassert>

namespace backtracking {

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int)
{
    assert(clues.size() % 4 == 0);

    std::size_t boardSize = clues.size() / 4;

    auto rowsData = getClueHints(clues, boardSize);

    Board board{boardSize};

    debug_print(board);

    return {};
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

} // namespace backtracking