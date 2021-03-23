#include "permutations.h"

#include "../shared/field.h"

#include <algorithm>
#include <numeric>

namespace permutation {

Permutations::Permutations(std::size_t size, Span<CluePair> cluePairs,
                           Span<Row> rows)
    : mCluePairs(cluePairs), mRows{rows}, mSize{size},
      mCluePairsPermutationIndexes(cluePairs.size())
{
    assert(cluePairs.size() == rows.size());
    std::vector<int> sequence(mSize);
    std::iota(sequence.begin(), sequence.end(), 1);
    mPermutationCount = factorial(sequence.size());

    mPermutations.reserve(mPermutationCount * mSize);
    int *p = &mPermutations[0];

    for (auto &cluePairPermutationIndexes : mCluePairsPermutationIndexes) {
        cluePairPermutationIndexes.reserve(mPermutationCount);
    }

    std::size_t currIndex = 0;
    do {
        addSequenceToCluePairs(sequence, currIndex);
        std::copy(sequence.begin(), sequence.end(), p);
        p += mSize;
        ++currIndex;
    } while (std::next_permutation(sequence.begin(), sequence.end()));
};

Span<int> Permutations::operator[](std::size_t elem) const
{
    auto ptr = &mPermutations[elem * mSize];
    return Span{ptr, mSize};
}

std::vector<std::size_t>
Permutations::permutationIndexs(std::size_t cluePairIndex) const
{
    return mCluePairsPermutationIndexes[cluePairIndex];
}

void Permutations::addSequenceToCluePairs(const std::vector<int> &sequence,
                                          int currIndex)
{
    auto front = buildingsVisible(sequence.cbegin(), sequence.cend());
    auto back = buildingsVisible(sequence.crbegin(), sequence.crend());

    for (std::size_t i = 0; i < mCluePairs.size(); ++i) {
        if (!permutationFitsCluePair(mCluePairs[i], front, back)) {
            continue;
        }
        auto fields = mRows[i].getFields();

        if (!existingSkyscrapersInPermutation(fields, sequence)) {
            continue;
        }
        mCluePairsPermutationIndexes[i].emplace_back(currIndex);
    }
}

bool Permutations::permutationFitsCluePair(const CluePair &cluePair, int front,
                                           int back)
{
    if (cluePair.isEmpty()) {
        return false;
    }
    if (!cluePair.frontIsEmpty() && cluePair.front() != front) {
        return false;
    }
    if (!cluePair.backIsEmpty() && cluePair.back() != back) {
        return false;
    }
    return true;
}

int factorial(int n)
{
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

bool existingSkyscrapersInPermutation(const std::vector<Field *> &fields,
                                      const std::vector<int> &permutation)
{
    assert(fields.size() == permutation.size());

    auto fieldIt = fields.cbegin();
    auto permutationIt = permutation.cbegin();

    for (; fieldIt != fields.cend() && permutationIt != permutation.cend();
         ++fieldIt, ++permutationIt) {
        if (!(*fieldIt)->hasSkyscraper()) {
            continue;
        }
        if ((*fieldIt)->skyscraper(fields.size()) != *permutationIt) {
            return false;
        }
    }
    return true;
}

} // namespace permutation
