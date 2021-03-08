#ifndef PERMUTATION_CLUEPAIR_H
#define PERMUTATION_CLUEPAIR_H

#include <vector>

namespace permutation {

class CluePair {
public:
    CluePair(int front, int back);

    int front() const;
    int back() const;

    bool frontIsEmpty() const;
    bool backIsEmpty() const;
    bool isEmpty() const;

private:
    int mFront;
    int mBack;
    bool mFrontIsEmpty;
    bool mBackIsEmpty;
    bool mIsEmpty;
};

std::vector<CluePair> makeCluePairs(const std::vector<int> &clues);

} // namespace permutation

#endif
