#include "backtracking.h"

#include "shared/board.h"
#include "shared/cluehints.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_set>

namespace backtracking {

int count = 0;

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

std::tuple<int, int> getRowClues(const std::vector<int> &clues, std::size_t y,
                                 std::size_t size)
{
    int frontClue = clues[clues.size() - 1 - y];
    int backClue = clues[size + y];
    return {frontClue, backClue};
}

std::tuple<int, int> getColumnClues(const std::vector<int> &clues,
                                    std::size_t x, std::size_t size)
{
    int frontClue = clues[x];
    int backClue = clues[size * 3 - 1 - x];
    return {frontClue, backClue};
}

template <typename Iterator> int visibleBuildings(Iterator begin, Iterator end)
{
    int visibleBuildingsCount = 0;
    int highestSeen = 0;
    for (auto it = begin; it != end; ++it) {
        if (*it != 0 && *it > highestSeen) {
            ++visibleBuildingsCount;
            highestSeen = *it;
        }
    }
    return visibleBuildingsCount;
}

bool rowCluesAreValid(const std::vector<std::vector<int>> &skyscrapers,
                      const std::vector<int> &clues, std::size_t y,
                      std::size_t size)
{
    auto [frontClue, backClue] = getRowClues(clues, y, size);

    if (frontClue == 0 && backClue == 0) {
        return true;
    }

    bool rowIsFull = std::find(skyscrapers[y].cbegin(), skyscrapers[y].cend(),
                               0) == skyscrapers[y].cend();

    if (!rowIsFull) {
        return true;
    }

    if (frontClue != 0) {
        auto frontVisible =
            visibleBuildings(skyscrapers[y].cbegin(), skyscrapers[y].cend());

        if (frontClue != frontVisible) {
            return false;
        }
    }
    if (backClue != 0) {
        auto backVisible =
            visibleBuildings(skyscrapers[y].crbegin(), skyscrapers[y].crend());

        if (backClue != backVisible) {
            return false;
        }
    }
    return true;
}

bool columnCluesAreValid(const std::vector<std::vector<int>> &skyscrapers,
                         const std::vector<int> &clues, std::size_t x,
                         std::size_t size)
{
    auto [frontClue, backClue] = getColumnClues(clues, x, size);

    if (frontClue == 0 && backClue == 0) {
        return true;
    }

    std::vector<int> verticalSkyscrapers;
    verticalSkyscrapers.reserve(size);

    for (std::size_t yi = 0; yi < size; ++yi) {
        verticalSkyscrapers.emplace_back(skyscrapers[yi][x]);
    }

    bool columnIsFull =
        std::find(verticalSkyscrapers.cbegin(), verticalSkyscrapers.cend(),
                  0) == verticalSkyscrapers.cend();

    if (!columnIsFull) {
        return true;
    }

    if (frontClue != 0) {
        auto frontVisible = visibleBuildings(verticalSkyscrapers.cbegin(),
                                             verticalSkyscrapers.cend());
        if (frontClue != frontVisible) {
            return false;
        }
    }
    if (backClue != 0) {
        auto backVisible = visibleBuildings(verticalSkyscrapers.crbegin(),
                                            verticalSkyscrapers.crend());

        if (backClue != backVisible) {
            return false;
        }
    }
    return true;
}

bool skyscrapersAreValidPositioned(
    const std::vector<std::vector<int>> &skyscrapers,
    const std::vector<int> &clues, std::size_t x, std::size_t y,
    std::size_t size)
{
    if (!rowsAreValid(skyscrapers, x, y, size)) {
        return false;
    }
    if (!columnsAreValid(skyscrapers, x, y, size)) {
        return false;
    }
    if (!rowCluesAreValid(skyscrapers, clues, y, size)) {
        return false;
    }
    if (!columnCluesAreValid(skyscrapers, clues, x, size)) {
        return false;
    }
    return true;
}

bool guessSkyscrapers(Board &board, const std::vector<int> &clues,
                      std::size_t x, std::size_t y, std::size_t size)
{
    ++count;

    if (x == size) {
        x = 0;
        y++;
    };
    if (y == size) {
        return true;
    }
    if (board.skyscrapers[y][x] != 0) {
        if (!skyscrapersAreValidPositioned(board.skyscrapers, clues, x, y,
                                           size)) {
            return false;
        }
        if (guessSkyscrapers(board, clues, x + 1, y, size)) {
            return true;
        }
        else {
            return false;
        }
    }

    for (int trySkyscraper = 1;
         trySkyscraper <= static_cast<int>(board.skyscrapers.size());
         ++trySkyscraper) {

        if (board.nopes[y][x].contains(trySkyscraper)) {
            continue;
        }

        board.skyscrapers[y][x] = trySkyscraper;

        if (!skyscrapersAreValidPositioned(board.skyscrapers, clues, x, y,
                                           size)) {
            continue;
        }

        if (guessSkyscrapers(board, clues, x + 1, y, size)) {
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

    guessSkyscrapers(board /*, rowsValues*/, clues, 0, 0,
                     board.skyscrapers.size());
    return board.skyscrapers;
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

} // namespace backtracking
