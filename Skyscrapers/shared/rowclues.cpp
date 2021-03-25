#include "rowclues.h"

#include <algorithm>
#include <cassert>
#include <unordered_set>

RowClues::RowClues(std::size_t boardSize)
    : fields{std::vector<Field>(boardSize, Field{})}
{
}

void RowClues::reverse()
{
    std::reverse(fields.begin(), fields.end());
}

bool RowClues::isEmpty() const
{
    return fields.empty();
}

std::vector<RowClues> getRowClues(const std::vector<int> &clues,
                                  std::size_t boardSize)
{
    std::vector<RowClues> rowClues;
    rowClues.reserve(clues.size());

    for (const auto &clue : clues) {
        rowClues.emplace_back(getRowClue(clue, boardSize));
    }
    mergeClueHintsPerRow(rowClues);
    return rowClues;
}

RowClues getRowClue(int clue, std::size_t boardSize)
{
    if (clue == 0) {
        return RowClues{};
    }

    RowClues rowClues{boardSize};

    if (clue == static_cast<int>(boardSize)) {
        for (std::size_t i = 0; i < boardSize; ++i) {
            rowClues.fields[i].insertSkyscraper(i + 1);
        }
    }
    else if (clue == 1) {
        rowClues.fields[0].insertSkyscraper(boardSize);
    }
    else if (clue == 2) {
        rowClues.fields[0].insertNope(boardSize);
        rowClues.fields[1].insertNope(boardSize - 1);
    }
    else {
        for (std::size_t fieldIdx = 0;
             fieldIdx < static_cast<std::size_t>(clue - 1); ++fieldIdx) {

            for (std::size_t nopeValue = boardSize;
                 nopeValue >= (boardSize - (clue - 2) + fieldIdx);
                 --nopeValue) {
                rowClues.fields[fieldIdx].insertNope(nopeValue);
            }
        }
    }
    return rowClues;
}

void mergeClueHintsPerRow(std::vector<RowClues> &rowClues)
{
    std::size_t startOffset = rowClues.size() / 4 * 3 - 1;
    std::size_t offset = startOffset;

    for (std::size_t frontIdx = 0; frontIdx < rowClues.size() / 2;
         ++frontIdx, offset -= 2) {

        if (frontIdx == rowClues.size() / 4) {
            offset = startOffset;
        }

        int backIdx = frontIdx + offset;

        rowClues[frontIdx] = merge(rowClues[frontIdx], rowClues[backIdx]);
    }
    rowClues.erase(rowClues.begin() + rowClues.size() / 2, rowClues.end());
}

RowClues merge(RowClues frontRowClues, RowClues backRowClues)
{
    if (frontRowClues.isEmpty() && backRowClues.isEmpty()) {
        return frontRowClues;
    }
    if (frontRowClues.isEmpty()) {
        backRowClues.reverse();
        return backRowClues;
    }
    if (backRowClues.isEmpty()) {
        return backRowClues;
    }

    assert(frontRowClues.fields.size() == backRowClues.fields.size());

    backRowClues.reverse();

    for (std::size_t i = 0; i < frontRowClues.fields.size(); ++i) {

        if (frontRowClues.fields[i].hasSkyscraper()) {
            continue;
        }
        else if (backRowClues.fields[i].hasSkyscraper()) {
            frontRowClues.fields[i].setBitmask(
                frontRowClues.fields[i].bitmask() |
                backRowClues.fields[i].bitmask());
        }
        else { // only nopes merge nopes
            frontRowClues.fields[i].insertNopes(backRowClues.fields[i]);
        }
    }
    return frontRowClues;
}
