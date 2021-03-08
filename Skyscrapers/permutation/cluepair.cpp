#include "cluepair.h"

namespace permutation {
CluePair::CluePair(int front, int back)
    : mFront{front}, mBack{back}, mFrontIsEmpty{mFront == 0},
      mBackIsEmpty{mBack == 0}, mIsEmpty{mFrontIsEmpty && mBackIsEmpty}
{
}

int CluePair::front() const
{
    return mFront;
}

int CluePair::back() const
{
    return mBack;
}

bool CluePair::frontIsEmpty() const
{
    return mFrontIsEmpty;
}

bool CluePair::backIsEmpty() const
{
    return mBackIsEmpty;
}

bool CluePair::isEmpty() const
{
    return mIsEmpty;
}

std::vector<CluePair> makeCluePairs(const std::vector<int> &clues)
{
    std::vector<CluePair> cluePairs;
    cluePairs.reserve(clues.size() / 2);

    std::size_t startOffset = clues.size() / 4 * 3 - 1;
    std::size_t offset = startOffset;

    for (std::size_t i = 0; i < clues.size() / 2; ++i, offset -= 2) {

        if (i == clues.size() / 4) {
            offset = startOffset;
        }

        int backClueIdx = i + offset;
        cluePairs.emplace_back(CluePair{clues[i], clues[backClueIdx]});
    }
    return cluePairs;
}

} // namespace permutation
