#include "backtracking.h"

#include "backtracking/algorithm.h"
#include "shared/board.h"
#include "shared/cluehints.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_set>

namespace backtracking {

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int)
{
    assert(clues.size() % 4 == 0);

    std::size_t boardSize = clues.size() / 4;

    auto clueHints = getClueHints(clues, boardSize);

    Board board{boardSize};

    board.insert(clueHints);
    board.insert(startingGrid);

    if (board.isSolved()) {
        return board.skyscrapers;
    }

    guessSkyscrapers(board, clues, 0, 0, board.skyscrapers.size());
    return board.skyscrapers;
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

} // namespace backtracking
