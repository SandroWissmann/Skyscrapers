#ifndef PERMUTATION_NOPES_H
#define PERMUTATION_NOPES_H

#include <numeric>
#include <unordered_set>
#include <vector>

namespace permutation {

template <typename It> int missingNumberInSequence(It begin, It end)
{
    int n = std::distance(begin, end) + 1;
    double projectedSum = (n + 1) * (n / 2.0);
    int actualSum = std::accumulate(begin, end, 0);
    return projectedSum - actualSum;
}

class Nopes {
public:
    Nopes(int size);

    void insert(int value);
    void insert(const std::vector<int> &values);
    bool sizeReached() const;
    int missingNumberInSequence() const;

    bool contains(int value) const;
    bool contains(const std::vector<int> &values);

    bool isEmpty() const;
    void clear();

    std::vector<int> containing() const;

    // for debug print
    std::unordered_set<int> values() const;

private:
    int mSize;
    std::unordered_set<int> mValues;
};
} // namespace permutation

#endif
