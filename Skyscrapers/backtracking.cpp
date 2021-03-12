#include "backtracking.h"

#include "shared/board.h"
#include "shared/cluehints.h"

#include <cassert>

namespace backtracking {

bool rowsAreValid(const std::vector<std::vector<int>> &skyscrapers,
                  std::size_t x, std::size_t y, std::size_t size)
{
    for (std::size_t xi = 0; xi < size; xi++) {
        if (xi != x && skyscrapers[y][xi] == skyscrapers[y][x]) {
            return false;
        }
    }
    return true;
}

bool columnsAreValid(const std::vector<std::vector<int>> &skyscrapers,
                     std::size_t x, std::size_t y, std::size_t size)
{
    for (std::size_t yi = 0; yi < size; yi++) {
        if (yi != y && skyscrapers[yi][x] == skyscrapers[y][x]) {
            return false;
        }
    }
    return true;
}

bool boardIsValid(const Board &board, std::size_t x, std::size_t y,
                  std::size_t size)
{
    if (!rowsAreValid(board.skyscrapers, x, y, size)) {
        return false;
    }
    if (!columnsAreValid(board.skyscrapers, x, y, size)) {
        return false;
    }
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
    if (board.skyscrapers[y][x] != 0) {
        if (!boardIsValid(board, x, y, size)) {
            return false;
        }
        return insertNextSkyscraper(board, x + 1, y, size);
    }

    for (board.skyscrapers[y][x] = 1;
         board.skyscrapers[y][x] <= static_cast<int>(board.skyscrapers.size());
         ++board.skyscrapers[y][x]) {

        if (!boardIsValid(board, x, y, size)) {
            continue;
        }
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
