#include "board.h"

#include "borderiterator.h"
#include "cluehints.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

Board::Board(std::size_t size)
    : skyscrapers{makeSkyscrapers(size)}, nopes{makeNopes(size)},
      mFields{std::vector<std::vector<Field>>(skyscrapers.size())}
{
    makeFields();
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

std::vector<std::vector<int>> Board::makeSkyscrapers(std::size_t size)
{
    std::vector<int> skyscraperRow(size, 0);
    return std::vector<std::vector<int>>(size, skyscraperRow);
}

std::vector<std::vector<Nopes>> Board::makeNopes(std::size_t size)
{
    std::vector<Nopes> nopesRow(size, Nopes{static_cast<int>(size) - 1});
    return std::vector<std::vector<Nopes>>(size, nopesRow);
}

void Board::makeFields()
{
    mFields.reserve(skyscrapers.size());
    for (auto &row : mFields) {
        row.reserve(mFields.size());
    }
    for (std::size_t y = 0; y < skyscrapers.size(); ++y) {
        mFields[y].reserve(skyscrapers.size());
        for (std::size_t x = 0; x < skyscrapers[y].size(); ++x) {
            mFields[y].emplace_back(Field{skyscrapers[y][x], nopes[y][x]});
        }
    }
}

void Board::makeRows()
{
    BorderIterator borderIterator{mFields.size()};

    std::size_t size = mFields.size() * 2;
    mRows.reserve(size);

    for (std::size_t i = 0; i < size; ++i, ++borderIterator) {
        mRows.emplace_back(Row{mFields, borderIterator.point(),
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
    for (std::size_t y = 0; y < board.skyscrapers.size(); ++y) {
        for (std::size_t x = 0; x < board.skyscrapers[y].size(); ++x) {

            if (board.skyscrapers[y][x] != 0) {
                std::cout << std::setw(board.skyscrapers.size() * 2);
                std::cout << "V" + std::to_string(board.skyscrapers[y][x]);
            }
            else if (board.skyscrapers[y][x] == 0 &&
                     !board.nopes[y][x].isEmpty()) {
                auto nopes_set = board.nopes[y][x].values();
                std::vector<int> nopes(nopes_set.begin(), nopes_set.end());
                std::sort(nopes.begin(), nopes.end());

                std::string nopesStr;
                for (std::size_t i = 0; i < nopes.size(); ++i) {
                    nopesStr.append(std::to_string(nopes[i]));
                    if (i != nopes.size() - 1) {
                        nopesStr.push_back(',');
                    }
                }
                std::cout << std::setw(board.skyscrapers.size() * 2);
                std::cout << nopesStr;
            }
            else {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}
