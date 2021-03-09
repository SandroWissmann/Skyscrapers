#include "readdirection.h"

#include "point.h"

#include <cassert>

namespace backtracking {
void nextDirection(ReadDirection &readDirection)
{
    assert(readDirection != ReadDirection::rightToLeft);
    int dir = static_cast<int>(readDirection);
    ++dir;
    readDirection = static_cast<ReadDirection>(dir);
}

void advanceToNextPosition(Point &point, ReadDirection readDirection,
                           int clueIdx)
{
    if (clueIdx == 0) {
        return;
    }
    switch (readDirection) {
    case ReadDirection::topToBottom:
        ++point.x;
        break;
    case ReadDirection::rightToLeft:
        ++point.y;
        break;
    }
}
} // namespace backtracking
