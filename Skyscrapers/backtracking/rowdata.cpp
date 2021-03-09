#include "rowdata.h"

#include <algorithm>
#include <cassert>

namespace backtracking {

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

void RowData::removeNopesOnSkyscrapers()
{
    for (std::size_t i = 0; i < skyscrapers.size(); ++i) {
        if (skyscrapers[i] == 0) {
            continue;
        }
        nopes[i].clear();
    }
}

std::vector<std::optional<RowData>>
getRowsDataFromClues(const std::vector<int> &clues, std::size_t boardSize)
{
    std::vector<std::optional<RowData>> rowsData;
    rowsData.reserve(clues.size());

    for (const auto &clue : clues) {
        rowsData.emplace_back(getRowDataFromClue(clue, boardSize));
    }
    mergeRowsDatafromFrontAndBack(rowsData);
    return rowsData;
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
    rowData.removeNopesOnSkyscrapers();
    return {rowData};
}

} // namespace backtracking
