#ifndef POINT_H
#define POINT_H

struct Point {
    int x;
    int y;
};

inline bool operator==(const Point &lhs, const Point &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
inline bool operator!=(const Point &lhs, const Point &rhs)
{
    return !(lhs == rhs);
}

#endif
