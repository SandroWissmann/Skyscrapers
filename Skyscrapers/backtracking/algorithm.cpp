#include "algorithm.h"

#include "../shared/board.h"

#include <algorithm>

namespace backtracking {
bool guessSkyscrapers(Board &board, const std::vector<int> &clues,
                      std::size_t index, std::size_t countOfElements,
                      std::size_t rowSize)
{
    if (++index == countOfElements) {
        return true;
    }
    if (board.skyscrapers[index] != 0) {
        if (!skyscrapersAreValidPositioned(board.skyscrapers, clues, index,
                                           rowSize)) {
            return false;
        }
        if (guessSkyscrapers(board, clues, index + 1, countOfElements,
                             rowSize)) {
            return true;
        }
        else {
            return false;
        }
    }

    for (int trySkyscraper = 1;
         trySkyscraper <= static_cast<int>(board.skyscrapers.size());
         ++trySkyscraper) {

        if (board.nopes[index].contains(trySkyscraper)) {
            continue;
        }
        board.skyscrapers[index] = trySkyscraper;
        if (!skyscrapersAreValidPositioned(board.skyscrapers, clues, index,
                                           rowSize)) {
            continue;
        }
        if (guessSkyscrapers(board, clues, index + 1, countOfElements,
                             rowSize)) {
            return true;
        }
    }
    board.skyscrapers[index] = 0;
    return false;
}

bool skyscrapersAreValidPositioned(const std::vector<int> &skyscrapers,
                                   const std::vector<int> &clues,
                                   std::size_t index, std::size_t rowSize)
{
    if (!rowsAreValid(skyscrapers, index, rowSize)) {
        return false;
    }
    if (!columnsAreValid(skyscrapers, index, rowSize)) {
        return false;
    }
    if (!rowCluesAreValid(skyscrapers, clues, index, rowSize)) {
        return false;
    }
    if (!columnCluesAreValid(skyscrapers, clues, index, rowSize)) {
        return false;
    }
    return true;
}

bool rowsAreValid(const std::vector<int> &skyscrapers, std::size_t index,
                  std::size_t rowSize)
{
    std::size_t row = index / rowSize;
    for (std::size_t currIndex = row * rowSize; currIndex < (row + 1) * rowSize;
         ++currIndex) {
        if (currIndex == index) {
            continue;
        }
        if (skyscrapers[currIndex] == skyscrapers[index]) {
            return false;
        }
    }
    return true;
}

bool columnsAreValid(const std::vector<int> &skyscrapers, std::size_t index,
                     std::size_t rowSize)
{
    std::size_t column = index % rowSize;

    for (std::size_t i = 0; i < rowSize; ++i) {
        std::size_t currIndex = column + i * rowSize;
        if (currIndex == index) {
            continue;
        }
        if (skyscrapers[currIndex] == skyscrapers[index]) {
            return false;
        }
    }
    return true;
}

bool rowCluesAreValid(const std::vector<int> &skyscrapers,
                      const std::vector<int> &clues, std::size_t index,
                      std::size_t rowSize)
{
    std::size_t row = index / rowSize;

    auto [frontClue, backClue] = getRowClues(clues, row, rowSize);

    if (frontClue == 0 && backClue == 0) {
        return true;
    }

    std::size_t rowIndexBegin = row * rowSize;
    std::size_t rowIndexEnd = (row + 1) * rowSize;

    auto citBegin = skyscrapers.cbegin() + rowIndexBegin;
    auto citEnd = skyscrapers.cbegin() + rowIndexEnd;

    bool rowIsFull = std::find(citBegin, citEnd, 0) == citEnd;

    if (!rowIsFull) {
        return true;
    }

    if (frontClue != 0) {
        auto frontVisible = visibleBuildings(citBegin, citEnd);

        if (frontClue != frontVisible) {
            return false;
        }
    }

    auto critBegin = std::make_reverse_iterator(citBegin);
    auto critEnd = std::make_reverse_iterator(citEnd);

    if (backClue != 0) {
        auto backVisible = visibleBuildings(critBegin, critEnd);

        if (backClue != backVisible) {
            return false;
        }
    }
    return true;
}

std::tuple<int, int> getRowClues(const std::vector<int> &clues, std::size_t row,
                                 std::size_t rowSize)
{
    int frontClue = clues[clues.size() - 1 - row];
    int backClue = clues[rowSize + row];
    return {frontClue, backClue};
}

bool columnCluesAreValid(const std::vector<int> &skyscrapers,
                         const std::vector<int> &clues, std::size_t index,
                         std::size_t rowSize)
{
    std::size_t column = index % rowSize;

    auto [frontClue, backClue] = getColumnClues(clues, index, rowSize);

    if (frontClue == 0 && backClue == 0) {
        return true;
    }

    std::vector<int> verticalSkyscrapers;
    verticalSkyscrapers.reserve(rowSize);

    for (std::size_t i = 0; i < rowSize; ++i) {
        verticalSkyscrapers.emplace_back(skyscrapers[column + i * rowSize]);
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

std::tuple<int, int> getColumnClues(const std::vector<int> &clues,
                                    std::size_t column, std::size_t rowSize)
{
    int frontClue = clues[column];
    int backClue = clues[rowSize * 3 - 1 - column];
    return {frontClue, backClue};
}
} // namespace backtracking
