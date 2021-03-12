#include "backtracking.h"

#include "shared/board.h"
#include "shared/cluehints.h"

#include <cassert>

namespace backtracking {

bool isValid()
{
    return true;
}

bool insertNextSkyscraper(Board &board, std::size_t x, std::size_t y,
                          std::size_t size)
{
    if (x == size) {
        x = 0;
        y++;
    };
    if (y == size) {
        return true;
    }
    if (board.skyscrapers[y][x] != 0) { // skyscraper present
        // invalid step?
        // -> return false
        return insertNextSkyscraper(board, x + 1, y, size);
    }

    for (board.skyscrapers[y][x] = 1;
         board.skyscrapers[y][x] <= static_cast<int>(board.skyscrapers.size());
         ++board.skyscrapers[y][x]) {

        // if invalid continue;

        if (insertNextSkyscraper(board, x + 1, y, size)) {
            return true;
        }
    }
    board.skyscrapers[y][x] = 0;
    return false;
}

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

    debug_print(board);

    insertNextSkyscraper(board, 0, 0, board.skyscrapers.size());

    debug_print(board);

    return board.skyscrapers;
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

} // namespace backtracking
