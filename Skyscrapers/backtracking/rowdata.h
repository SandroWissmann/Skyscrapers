#ifndef BACKTRACKING_ROWDATA_H
#define BACKTRACKING_ROWDATA_H

#include <cstddef>
#include <unordered_set>
#include <vector>

namespace backtracking {
struct RowData {
    RowData(std::size_t boardSize);
    RowData();

    void reverse();

    void removeNopesOnSkyscrapers();

    std::vector<int> skyscrapers{};
    std::vector<std::unordered_set<int>> nopes{};
};

std::vector<std::optional<RowData>>
getRowsDataFromClues(const std::vector<int> &clues, std::size_t boardSize);

std::optional<RowData> getRowDataFromClue(int clue, std::size_t boardSize);

void mergeRowsDatafromFrontAndBack(
    std::vector<std::optional<RowData>> &rowsData);

std::optional<RowData> merge(std::optional<RowData> optFrontRowData,
                             std::optional<RowData> optBackRowData);

} // namespace backtracking
#endif
