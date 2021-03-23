#include "cluehints.h"

#include <algorithm>
#include <cassert>
#include <unordered_set>

ClueHints::ClueHints(std::size_t boardSize)
    : fields{std::vector<Field>(boardSize, Field{})}
{
}

void ClueHints::reverse()
{
    std::reverse(fields.begin(), fields.end());
}

bool ClueHints::isEmpty() const
{
    return fields.empty();
}

std::vector<ClueHints> getClueHints(const std::vector<int> &clues,
                                    std::size_t boardSize)
{
    std::vector<ClueHints> clueHints;
    clueHints.reserve(clues.size());

    for (const auto &clue : clues) {
        clueHints.emplace_back(getClueHints(clue, boardSize));
    }
    mergeClueHintsPerRow(clueHints);
    return clueHints;
}

ClueHints getClueHints(int clue, std::size_t boardSize)
{
    if (clue == 0) {
        return ClueHints{};
    }

    ClueHints clueHints{boardSize};

    if (clue == static_cast<int>(boardSize)) {
        for (std::size_t i = 0; i < boardSize; ++i) {
            clueHints.fields[i].insertSkyscraper(i + 1);
        }
    }
    else if (clue == 1) {
        clueHints.fields[0].insertSkyscraper(boardSize);
    }
    else if (clue == 2) {
        clueHints.fields[0].insertNope(boardSize);
        clueHints.fields[1].insertNope(boardSize - 1);
    }
    else {
        for (std::size_t fieldIdx = 0;
             fieldIdx < static_cast<std::size_t>(clue - 1); ++fieldIdx) {

            for (std::size_t nopeValue = boardSize;
                 nopeValue >= (boardSize - (clue - 2) + fieldIdx);
                 --nopeValue) {
                clueHints.fields[fieldIdx].insertNope(nopeValue);
            }
        }
    }
    return clueHints;
}

void mergeClueHintsPerRow(std::vector<ClueHints> &clueHints)
{
    std::size_t startOffset = clueHints.size() / 4 * 3 - 1;
    std::size_t offset = startOffset;

    for (std::size_t frontIdx = 0; frontIdx < clueHints.size() / 2;
         ++frontIdx, offset -= 2) {

        if (frontIdx == clueHints.size() / 4) {
            offset = startOffset;
        }

        int backIdx = frontIdx + offset;

        clueHints[frontIdx] = merge(clueHints[frontIdx], clueHints[backIdx]);
    }
    clueHints.erase(clueHints.begin() + clueHints.size() / 2, clueHints.end());
}

ClueHints merge(ClueHints frontClueHints, ClueHints backClueHints)
{
    if (frontClueHints.isEmpty() && backClueHints.isEmpty()) {
        return frontClueHints;
    }
    if (frontClueHints.isEmpty()) {
        backClueHints.reverse();
        return backClueHints;
    }
    if (backClueHints.isEmpty()) {
        return backClueHints;
    }

    assert(frontClueHints.fields.size() == backClueHints.fields.size());

    backClueHints.reverse();

    for (std::size_t i = 0; i < frontClueHints.fields.size(); ++i) {
        frontClueHints.fields[i].setBitmask(frontClueHints.fields[i].bitmask() |
                                            backClueHints.fields[i].bitmask());
    }
    return frontClueHints;
}
