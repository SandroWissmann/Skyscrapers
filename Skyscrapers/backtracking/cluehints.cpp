#include "cluehints.h"

#include <algorithm>
#include <cassert>
#include <unordered_set>

namespace backtracking {

ClueHints::ClueHints()
{
}

ClueHints::ClueHints(std::size_t boardSize)
    : skyscrapers{std::vector<int>(boardSize, 0)},
      nopes{std::vector<std::vector<int>>(boardSize, std::vector<int>{})}
{
}

void ClueHints::reverse()
{
    std::reverse(skyscrapers.begin(), skyscrapers.end());
    std::reverse(nopes.begin(), nopes.end());
}

void ClueHints::removeNopesOnSkyscrapers()
{
    for (std::size_t i = 0; i < skyscrapers.size(); ++i) {
        if (skyscrapers[i] == 0) {
            continue;
        }
        nopes[i].clear();
    }
}

std::vector<std::optional<ClueHints>>
getClueHints(const std::vector<int> &clues, std::size_t boardSize)
{
    std::vector<std::optional<ClueHints>> clueHints;
    clueHints.reserve(clues.size());

    for (const auto &clue : clues) {
        clueHints.emplace_back(getClueHints(clue, boardSize));
    }
    mergeClueHintsPerRow(clueHints);
    return clueHints;
}

std::optional<ClueHints> getClueHints(int clue, std::size_t boardSize)
{
    if (clue == 0) {
        return {};
    }

    ClueHints clueHints{boardSize};

    std::vector<std::unordered_set<int>> nopes(boardSize,
                                               std::unordered_set<int>{});

    if (clue == static_cast<int>(boardSize)) {
        for (std::size_t i = 0; i < boardSize; ++i) {
            clueHints.skyscrapers[i] = i + 1;
        }
    }
    else if (clue == 1) {
        clueHints.skyscrapers[0] = boardSize;
    }
    else if (clue == 2) {
        nopes[0].insert(boardSize);
        nopes[1].insert(boardSize - 1);
    }
    else {
        for (std::size_t fieldIdx = 0;
             fieldIdx < static_cast<std::size_t>(clue - 1); ++fieldIdx) {

            for (std::size_t nopeValue = boardSize;
                 nopeValue >= (boardSize - (clue - 2) + fieldIdx);
                 --nopeValue) {
                nopes[fieldIdx].insert(nopeValue);
            }
        }
    }

    assert(nopes.size() == clueHints.nopes.size());

    for (std::size_t i = 0; i < nopes.size(); ++i) {
        clueHints.nopes[i] = std::vector<int>(nopes[i].begin(), nopes[i].end());
    }
    return {clueHints};
}

void mergeClueHintsPerRow(std::vector<std::optional<ClueHints>> &clueHints)
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

std::optional<ClueHints> merge(std::optional<ClueHints> optFrontClueHints,
                               std::optional<ClueHints> optBackClueHints)
{
    if (!optFrontClueHints && !optBackClueHints) {
        return {};
    }
    if (!optFrontClueHints) {
        optBackClueHints->reverse();
        return optBackClueHints;
    }
    if (!optBackClueHints) {
        return optFrontClueHints;
    }

    auto size = optFrontClueHints->skyscrapers.size();
    ClueHints clueHints{size};

    assert(optFrontClueHints->skyscrapers.size() ==
           optFrontClueHints->nopes.size());
    assert(optBackClueHints->skyscrapers.size() ==
           optBackClueHints->nopes.size());
    assert(optFrontClueHints->skyscrapers.size() ==
           optBackClueHints->skyscrapers.size());

    optBackClueHints->reverse();

    for (std::size_t i = 0; i < optFrontClueHints->skyscrapers.size(); ++i) {

        auto frontSkyscraper = optFrontClueHints->skyscrapers[i];
        auto backSkyscraper = optBackClueHints->skyscrapers[i];

        if (frontSkyscraper != 0 && backSkyscraper != 0) {
            assert(frontSkyscraper == backSkyscraper);
            clueHints.skyscrapers[i] = frontSkyscraper;
        }
        else if (frontSkyscraper != 0) {
            clueHints.skyscrapers[i] = frontSkyscraper;
            clueHints.nopes[i].clear();
        }
        else { // backSkyscraper != 0
            clueHints.skyscrapers[i] = backSkyscraper;
            clueHints.nopes[i].clear();
        }

        if (clueHints.skyscrapers[i] != 0) {
            continue;
        }

        std::unordered_set<int> nopes(optFrontClueHints->nopes[i].begin(),
                                      optFrontClueHints->nopes[i].end());
        nopes.insert(optBackClueHints->nopes[i].begin(),
                     optBackClueHints->nopes[i].end());
        clueHints.nopes[i] = std::vector<int>(nopes.begin(), nopes.end());
    }
    clueHints.removeNopesOnSkyscrapers();
    return {clueHints};
}

} // namespace backtracking
