#ifndef PERMUTATION_BORDERITERATOR_H
#define PERMUTATION_BORDERITERATOR_H

#include "point.h"
#include "readdirection.h"

#include <cstddef>

namespace permutation {

class BorderIterator {
public:
    BorderIterator(std::size_t boardSize);

    Point point() const;
    ReadDirection readDirection() const;

    BorderIterator &operator++();

private:
    int mIdx = 0;
    std::size_t mBoardSize;
    Point mPoint{0, 0};
    ReadDirection mReadDirection{ReadDirection::topToBottom};
};

} // namespace permutation

#endif
