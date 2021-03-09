#ifndef BACKTRACKING_READDIRECTION_H
#define BACKTRACKING_READDIRECTION_H

namespace backtracking {

struct Point;

enum class ReadDirection { topToBottom, rightToLeft };

void nextDirection(ReadDirection &readDirection);

void advanceToNextPosition(Point &point, ReadDirection readDirection,
                           int clueIdx);
} // namespace backtracking

#endif
