#ifndef BACKTRACKING_CLUEHINTS_H
#define BACKTRACKING_CLUEHINTS_H

#include <cstddef>
#include <optional>
#include <vector>

namespace backtracking {
struct ClueHints {
    ClueHints(std::size_t boardSize);
    ClueHints();

    void reverse();

    void removeNopesOnSkyscrapers();

    std::vector<int> skyscrapers{};
    std::vector<std::vector<int>> nopes{};
};

std::vector<std::optional<ClueHints>>
getClueHints(const std::vector<int> &clues, std::size_t boardSize);

std::optional<ClueHints> getClueHints(int clue, std::size_t boardSize);

void mergeClueHintsPerRow(std::vector<std::optional<ClueHints>> &clueHints);

std::optional<ClueHints> merge(std::optional<ClueHints> optFrontClueHints,
                               std::optional<ClueHints> optBackClueHints);

} // namespace backtracking
#endif
