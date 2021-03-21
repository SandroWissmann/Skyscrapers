#include "board.h"

#include "borderiterator.h"
#include "cluehints.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <numeric>

Board::Board(std::size_t size)
    : fields{std::vector<Field>(size * size, Field{})}, mSize{size}
{
    makeRows();
}

void Board::insert(const std::vector<std::optional<ClueHints>> &clueHints)
{
    assert(clueHints.size() == mRows.size());

    for (std::size_t i = 0; i < clueHints.size(); ++i) {
        if (!clueHints[i]) {
            continue;
        }
        mRows[i].addNopes(clueHints[i]->nopes, Row::Direction::front);
        mRows[i].addSkyscrapers(clueHints[i]->skyscrapers,
                                Row::Direction::front);
    }
}

void Board::insert(const std::vector<std::vector<int>> &startingSkyscrapers)
{
    if (startingSkyscrapers.empty()) {
        return;
    }
    std::size_t boardSize = mRows.size() / 2;
    assert(startingSkyscrapers.size() == boardSize);

    for (std::size_t i = 0; i < startingSkyscrapers.size(); ++i) {
        mRows[i + boardSize].addSkyscrapers(startingSkyscrapers[i],
                                            Row::Direction::back);
    }
}

bool Board::isSolved() const
{
    std::size_t endVerticalRows = mRows.size() / 2;
    for (std::size_t i = 0; i < endVerticalRows; ++i) {
        if (!mRows[i].allFieldsContainSkyscraper()) {
            return false;
        }
    }
    return true;
}

std::vector<std::vector<int>> Board::skyscrapers2d() const
{
    std::vector<std::vector<int>> skyscrapers2d(mSize, std::vector<int>());

    std::size_t j = 0;
    skyscrapers2d[j].reserve(mSize);
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (i != 0 && i % mSize == 0) {
            ++j;
            skyscrapers2d[j].reserve(mSize);
        }
        skyscrapers2d[j].emplace_back(fields[i].skyscraper(mSize));
    }
    return skyscrapers2d;
}

std::size_t Board::size() const
{
    return mSize;
}

void Board::makeRows()
{
    BorderIterator borderIterator{mSize};

    std::size_t rowSize = mSize * 2;
    mRows.reserve(rowSize);

    for (std::size_t i = 0; i < rowSize; ++i, ++borderIterator) {
        mRows.emplace_back(Row{fields, mSize, borderIterator.point(),
                               borderIterator.readDirection()});
    }
    connnectRowsWithCrossingRows();
}

void Board::connnectRowsWithCrossingRows()
{
    std::size_t boardSize = mRows.size() / 2;

    std::vector<int> targetRowsIdx(boardSize);
    std::iota(targetRowsIdx.begin(), targetRowsIdx.end(), boardSize);

    for (std::size_t i = 0; i < mRows.size(); ++i) {
        if (i == mRows.size() / 2) {
            std::iota(targetRowsIdx.begin(), targetRowsIdx.end(), 0);
            std::reverse(targetRowsIdx.begin(), targetRowsIdx.end());
        }

        for (const auto &targetRowIdx : targetRowsIdx) {
            mRows[i].addCrossingRows(&mRows[targetRowIdx]);
        }
    }
}

void debug_print(Board &board, const std::string &title)
{
    std::cout << title << '\n';

    for (std::size_t i = 0; i < board.fields.size(); ++i) {

        if (i % board.size() == 0 && i != 0) {
            std::cout << '\n';
        }

        if (board.fields[i].skyscraper(board.size()) != 0) {
            std::cout << std::setw(board.size() * 2);
            std::cout << "V" + std::to_string(
                                   board.fields[i].skyscraper(board.size()));
        }
        else if (board.fields[i].skyscraper(board.size()) == 0 &&
                 !board.fields[i].nopes(board.size()).empty()) {
            auto nopes_set = board.fields[i].nopes(board.size());
            std::vector<int> nopes(nopes_set.begin(), nopes_set.end());
            std::sort(nopes.begin(), nopes.end());

            std::string nopesStr;
            for (std::size_t i = 0; i < nopes.size(); ++i) {
                nopesStr.append(std::to_string(nopes[i]));
                if (i != nopes.size() - 1) {
                    nopesStr.push_back(',');
                }
            }
            std::cout << std::setw(board.size() * 2);
            std::cout << nopesStr;
        }
        else {
            std::cout << ' ';
        }
    }
    std::cout << '\n';
}
