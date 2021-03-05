#include "backtracking.h"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <optional>
#include <unordered_set>
#include <vector>

#include <iomanip>
#include <iostream>

namespace backtracking {

template <typename It> int missingNumberInSequence(It begin, It end)
{
    int n = std::distance(begin, end) + 1;
    double projectedSum = (n + 1) * (n / 2.0);
    int actualSum = std::accumulate(begin, end, 0);
    return projectedSum - actualSum;
}

class Nopes {
public:
    Nopes(int size);

    void insert(int value);
    void insert(const std::vector<int> &values);
    bool sizeReached() const;
    int missingNumberInSequence() const;

    bool contains(int value) const;
    bool contains(const std::vector<int> &values);

    bool isEmpty() const;
    void clear();

    std::vector<int> containing() const;

    // for debug print
    std::unordered_set<int> values() const;

private:
    int mSize;
    std::unordered_set<int> mValues;
};

Nopes::Nopes(int size) : mSize{size}
{
    assert(size > 0);
}

void Nopes::insert(int value)
{
    assert(value >= 1 && value <= mSize + 1);
    mValues.insert(value);
}

void Nopes::insert(const std::vector<int> &values)
{
    mValues.insert(values.begin(), values.end());
}

bool Nopes::sizeReached() const
{
    return mValues.size() == static_cast<std::size_t>(mSize);
}

int Nopes::missingNumberInSequence() const
{
    assert(sizeReached());
    return backtracking::missingNumberInSequence(mValues.begin(),
                                                 mValues.end());
}

bool Nopes::contains(int value) const
{
    auto it = mValues.find(value);
    return it != mValues.end();
}

bool Nopes::contains(const std::vector<int> &values)
{
    for (const auto &value : values) {
        if (!contains(value)) {
            return false;
        }
    }
    return true;
}

bool Nopes::isEmpty() const
{
    return mValues.empty();
}

void Nopes::clear()
{
    mValues.clear();
}

std::vector<int> Nopes::containing() const
{
    std::vector<int> nopes;
    nopes.reserve(mValues.size());
    for (const auto &value : mValues) {
        nopes.emplace_back(value);
    }
    return nopes;
}

std::unordered_set<int> Nopes::values() const
{
    return mValues;
}

struct Board {
    Board(int size);

    std::vector<std::vector<int>> skyscrapers{};
    std::vector<std::vector<Nopes>> nopes;

private:
    std::vector<std::vector<int>> makeSkyscrapers(int size);
    std::vector<std::vector<Nopes>> makeNopes(int size);
};

Board::Board(int size)
    : skyscrapers{makeSkyscrapers(size)}, nopes{makeNopes(size)}
{
}

std::vector<std::vector<int>> Board::makeSkyscrapers(int size)
{
    std::vector<int> skyscraperRow(size, 0);
    return std::vector<std::vector<int>>(size, skyscraperRow);
}

std::vector<std::vector<Nopes>> Board::makeNopes(int size)
{
    std::vector<Nopes> nopesRow(size, Nopes{size - 1});
    return std::vector<std::vector<Nopes>>(size, nopesRow);
}

void debug_print(Board &board, const std::string &title = "")
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

struct RowData {
    RowData(std::size_t boardSize);
    RowData();

    void reverse();

    void shrinkToFit();
    void removeNopesOnSkyscrapers();

    std::vector<int> skyscrapers{};
    std::vector<std::unordered_set<int>> nopes{};
};

RowData::RowData()
{
}

RowData::RowData(std::size_t boardSize)
    : skyscrapers{std::vector<int>(boardSize, 0)},
      nopes{std::vector<std::unordered_set<int>>(boardSize,
                                                 std::unordered_set<int>{})}
{
}

void RowData::reverse()
{
    std::reverse(skyscrapers.begin(), skyscrapers.end());
    std::reverse(nopes.begin(), nopes.end());
}

void RowData::shrinkToFit()
{
}

void RowData::removeNopesOnSkyscrapers()
{
}

std::optional<RowData> getRowDataFromClue(int clue, std::size_t boardSize)
{
    if (clue == 0) {
        return {};
    }

    RowData rowData{boardSize};
    if (clue == static_cast<int>(boardSize)) {
        for (std::size_t i = 0; i < boardSize; ++i) {
            rowData.skyscrapers[i] = i + 1;
        }
    }
    else if (clue == 1) {
        rowData.skyscrapers[0] = boardSize;
    }
    else if (clue == 2) {
        rowData.nopes[0].insert(boardSize);
        rowData.nopes[1].insert(boardSize - 1);
    }
    else {
        for (std::size_t fieldIdx = 0;
             fieldIdx < static_cast<std::size_t>(clue - 1); ++fieldIdx) {

            for (std::size_t nopeValue = boardSize;
                 nopeValue >= (boardSize - (clue - 2) + fieldIdx);
                 --nopeValue) {
                rowData.nopes[fieldIdx].insert(nopeValue);
            }
        }
    }
    return {rowData};
}

std::vector<std::optional<RowData>>
getRowsDataFromClues(const std::vector<int> &clues, std::size_t boardSize)
{
    std::vector<std::optional<RowData>> rowsData;
    rowsData.reserve(clues.size());

    for (const auto &clue : clues) {
        rowsData.emplace_back(getRowDataFromClue(clue, boardSize));
    }
    return rowsData;
}

std::optional<RowData> merge(std::optional<RowData> optFrontRowData,
                             std::optional<RowData> optBackRowData)
{
    if (!optFrontRowData && !optBackRowData) {
        return {};
    }
    if (!optFrontRowData) {
        optBackRowData->reverse();
        return optBackRowData;
    }
    if (!optBackRowData) {
        return optFrontRowData;
    }

    auto size = optFrontRowData->skyscrapers.size();
    RowData rowData{size};

    assert(optFrontRowData->skyscrapers.size() ==
           optFrontRowData->nopes.size());
    assert(optBackRowData->skyscrapers.size() == optBackRowData->nopes.size());
    assert(optFrontRowData->skyscrapers.size() ==
           optBackRowData->skyscrapers.size());

    optBackRowData->reverse();

    for (std::size_t i = 0; i < optFrontRowData->skyscrapers.size(); ++i) {

        auto frontSkyscraper = optFrontRowData->skyscrapers[i];
        auto backSkyscraper = optBackRowData->skyscrapers[i];

        if (frontSkyscraper != 0 && backSkyscraper != 0) {
            assert(frontSkyscraper == backSkyscraper);
            rowData.skyscrapers[i] = frontSkyscraper;
        }
        else if (frontSkyscraper != 0) {
            rowData.skyscrapers[i] = frontSkyscraper;
            rowData.nopes[i].clear();
        }
        else { // backSkyscraper != 0
            rowData.skyscrapers[i] = backSkyscraper;
            rowData.nopes[i].clear();
        }

        if (rowData.skyscrapers[i] != 0) {
            continue;
        }
        rowData.nopes[i].insert(optFrontRowData->nopes[i].begin(),
                                optFrontRowData->nopes[i].end());
        rowData.nopes[i].insert(optBackRowData->nopes[i].begin(),
                                optBackRowData->nopes[i].end());
    }
    return {rowData};
}

void mergeRowsDatafromFrontAndBack(
    std::vector<std::optional<RowData>> &rowsData)
{
    std::size_t startOffset = rowsData.size() / 4 * 3 - 1;
    std::size_t offset = startOffset;

    for (std::size_t frontIdx = 0; frontIdx < rowsData.size() / 2;
         ++frontIdx, offset -= 2) {

        if (frontIdx == rowsData.size() / 4) {
            offset = startOffset;
        }

        int backIdx = frontIdx + offset;

        rowsData[frontIdx] = merge(rowsData[frontIdx], rowsData[backIdx]);
    }
    rowsData.erase(rowsData.begin() + rowsData.size() / 2, rowsData.end());
}

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int)
{
    assert(clues.size() % 4 == 0);

    std::size_t boardSize = clues.size() / 4;

    auto rowsData = getRowsDataFromClues(clues, boardSize);
    mergeRowsDatafromFrontAndBack(rowsData);

    return {};
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

} // namespace backtracking
