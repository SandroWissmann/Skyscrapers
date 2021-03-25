#include "algorithm.h"

#include "../shared/board.h"

#include <algorithm>

#include <iostream>

namespace backtracking {

// int count = 0;

bool guessSkyscrapers(Board &board, const std::vector<int> &clues,
                      std::size_t index, std::size_t countOfElements,
                      std::size_t rowSize)
{
    // debug_print(board);
    //++count;
    if (index == countOfElements) {
        // std::cout << count << '\n';
        // count = 0;
        return true;
    }

    if (board.fields[index].hasSkyscraper()) {
        if (!skyscrapersAreValidPositioned(board.fields, clues, index,
                                           rowSize)) {
            return false;
        }
        if (guessSkyscrapers(board, clues, index + 1, countOfElements,
                             rowSize)) {
            return true;
        }
        return false;
    }

    auto oldBitmask = board.fields[index].bitmask();
    for (int trySkyscraper = 1; trySkyscraper <= static_cast<int>(rowSize);
         ++trySkyscraper) {

        if (board.fields[index].containsNope(trySkyscraper)) {
            continue;
        }
        board.fields[index].insertSkyscraper(trySkyscraper);
        if (!skyscrapersAreValidPositioned(board.fields, clues, index,
                                           rowSize)) {
            board.fields[index].setBitmask(oldBitmask);
            continue;
        }
        if (guessSkyscrapers(board, clues, index + 1, countOfElements,
                             rowSize)) {
            return true;
        }
        board.fields[index].setBitmask(oldBitmask);
    }
    board.fields[index].setBitmask(oldBitmask);
    return false;
}

bool skyscrapersAreValidPositioned(const std::vector<Field> &fields,
                                   const std::vector<int> &clues,
                                   std::size_t index, std::size_t rowSize)
{
    if (!rowsAreValid(fields, index, rowSize)) {
        return false;
    }
    if (!columnsAreValid(fields, index, rowSize)) {
        return false;
    }
    if (!cluesInRowAreValid(fields, clues, index, rowSize)) {
        return false;
    }
    if (!cluesInColumnAreValid(fields, clues, index, rowSize)) {
        return false;
    }
    return true;
}

bool rowsAreValid(const std::vector<Field> &fields, std::size_t index,
                  std::size_t rowSize)
{
    std::size_t row = index / rowSize;
    for (std::size_t currIndex = row * rowSize; currIndex < (row + 1) * rowSize;
         ++currIndex) {
        if (currIndex == index) {
            continue;
        }
        if (fields[currIndex].skyscraper(rowSize) ==
            fields[index].skyscraper(rowSize)) {
            return false;
        }
    }
    return true;
}

bool columnsAreValid(const std::vector<Field> &fields, std::size_t index,
                     std::size_t rowSize)
{
    std::size_t column = index % rowSize;

    for (std::size_t i = 0; i < rowSize; ++i) {
        std::size_t currIndex = column + i * rowSize;
        if (currIndex == index) {
            continue;
        }
        if (fields[currIndex].skyscraper(rowSize) ==
            fields[index].skyscraper(rowSize)) {
            return false;
        }
    }
    return true;
}

bool cluesInRowAreValid(const std::vector<Field> &fields,
                        const std::vector<int> &clues, std::size_t index,
                        std::size_t rowSize)
{
    std::size_t row = index / rowSize;

    auto [frontClue, backClue] = getCluesInRow(clues, row, rowSize);

    if (frontClue == 0 && backClue == 0) {
        return true;
    }

    std::size_t rowIndexBegin = row * rowSize;
    std::size_t rowIndexEnd = (row + 1) * rowSize;

    auto citBegin = fields.cbegin() + rowIndexBegin;
    auto citEnd = fields.cbegin() + rowIndexEnd;

    bool rowIsFull = std::find_if(citBegin, citEnd, [](const Field &field) {
                         return !field.hasSkyscraper();
                     }) == citEnd;

    if (!rowIsFull) {
        return true;
    }

    if (frontClue != 0) {
        auto frontVisible = visibleBuildings(citBegin, citEnd, rowSize);

        if (frontClue != frontVisible) {
            return false;
        }
    }

    auto critBegin = std::make_reverse_iterator(citEnd);
    auto critEnd = std::make_reverse_iterator(citBegin);

    if (backClue != 0) {
        auto backVisible = visibleBuildings(critBegin, critEnd, rowSize);

        if (backClue != backVisible) {
            return false;
        }
    }
    return true;
}

std::tuple<int, int> getCluesInRow(const std::vector<int> &clues,
                                   std::size_t row, std::size_t rowSize)
{
    int frontClue = clues[clues.size() - 1 - row];
    int backClue = clues[rowSize + row];
    return {frontClue, backClue};
}

bool cluesInColumnAreValid(const std::vector<Field> &fields,
                           const std::vector<int> &clues, std::size_t index,
                           std::size_t rowSize)
{
    std::size_t column = index % rowSize;

    auto [frontClue, backClue] = getCluesInColumn(clues, column, rowSize);

    if (frontClue == 0 && backClue == 0) {
        return true;
    }

    std::vector<Field> verticalFields;
    verticalFields.reserve(rowSize);

    for (std::size_t i = 0; i < rowSize; ++i) {
        verticalFields.emplace_back(fields[column + i * rowSize]);
    }

    bool columnIsFull =
        std::find_if(verticalFields.cbegin(), verticalFields.cend(),
                     [](const Field &field) {
                         return !field.hasSkyscraper();
                     }) == verticalFields.cend();

    if (!columnIsFull) {
        return true;
    }

    if (frontClue != 0) {
        auto frontVisible = visibleBuildings(verticalFields.cbegin(),
                                             verticalFields.cend(), rowSize);
        if (frontClue != frontVisible) {
            return false;
        }
    }
    if (backClue != 0) {
        auto backVisible = visibleBuildings(verticalFields.crbegin(),
                                            verticalFields.crend(), rowSize);

        if (backClue != backVisible) {
            return false;
        }
    }
    return true;
}

std::tuple<int, int> getCluesInColumn(const std::vector<int> &clues,
                                      std::size_t column, std::size_t rowSize)
{
    int frontClue = clues[column];
    int backClue = clues[rowSize * 3 - 1 - column];
    return {frontClue, backClue};
}
} // namespace backtracking
