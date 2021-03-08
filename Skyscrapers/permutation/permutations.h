#ifndef PERMUTATION_PERMUTATIONS_H
#define PERMUTATION_PERMUTATIONS_H

#include "cluepair.h"
#include "row.h"
#include "span.h"

#include <vector>

namespace permutation {

class Permutations {
public:
    Permutations(std::size_t size, Span<CluePair> cluePairs, Span<Row> rows);

    Span<int> operator[](std::size_t permutationIndex) const;

    std::vector<std::size_t> permutationIndexs(std::size_t cluePairIndex) const;

private:
    void addSequenceToCluePairs(const std::vector<int> &sequence,
                                int currIndex);

    bool permutationFitsCluePair(const CluePair &cluePair, int front, int back);

    Span<CluePair> mCluePairs;
    Span<Row> mRows;

    std::size_t mSize;
    std::size_t mPermutationCount;
    std::vector<std::vector<std::size_t>> mCluePairsPermutationIndexes;

    std::vector<int> mPermutations;
};

int factorial(int n);

template <typename BuildingIt>
int buildingsVisible(BuildingIt begin, BuildingIt end)
{
    int visibleBuildingsCount = 0;
    int highestSeen = 0;

    for (auto it = begin; it != end; ++it) {
        if (*it > highestSeen) {
            ++visibleBuildingsCount;
            highestSeen = *it;
        }
    }
    return visibleBuildingsCount;
}

bool existingSkyscrapersInPermutation(const std::vector<Field *> &fields,
                                      const std::vector<int> &permutation);

} // namespace permutation

#endif
