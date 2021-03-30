#include "backtracking.h"

#include "backtracking/algorithm.h"
#include "shared/board.h"
#include "shared/rowclues.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace backtracking {

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int)
{
    assert(clues.size() % 4 == 0);

    std::size_t boardSize = clues.size() / 4;

    auto rowClues = getRowClues(clues, boardSize);

    Board board{boardSize};

    board.insert(rowClues);
    board.insert(startingGrid);

    if (board.isSolved()) {
        return board.skyscrapers2d();
    }

    solveBoard(board, clues);

    return board.skyscrapers2d();
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

void solveBoard(Board &board, const std::vector<int> &clues)
{
    guessSkyscrapers(board, clues, 0, board.fields.size(), board.size());
}

} // namespace backtracking
