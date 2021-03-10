#ifndef READDIRECTION_H
#define READDIRECTION_H

struct Point;

enum class ReadDirection { topToBottom, rightToLeft };

void nextDirection(ReadDirection &readDirection);

void advanceToNextPosition(Point &point, ReadDirection readDirection,
                           int clueIdx);

#endif
