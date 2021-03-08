#ifndef PERMUTATION_READDIRECTION_H
#define PERMUTATION_READDIRECTION_H

namespace permutation {

struct Point;

enum class ReadDirection { topToBottom, rightToLeft };

void nextDirection(ReadDirection &readDirection);

void advanceToNextPosition(Point &point, ReadDirection readDirection,
                           int clueIdx);
} // namespace permutation

#endif
