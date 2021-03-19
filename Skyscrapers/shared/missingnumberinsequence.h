#ifndef MISSINGNUMBERINSEQUENCE_H
#define MISSINGNUMBERINSEQUENCE_H

#include <numeric>

template <typename It> int missingNumberInSequence(It begin, It end)
{
    int n = std::distance(begin, end) + 1;
    double projectedSum = (n + 1) * (n / 2.0);
    int actualSum = std::accumulate(begin, end, 0);
    return projectedSum - actualSum;
}

#endif
