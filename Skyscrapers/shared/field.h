#ifndef FIELD_H
#define FIELD_H

#include <cstdint>
#include <vector>

using BitmaskType = std::uint32_t;

/*
Example size = 4

b0000 0 Nopes = {}
b0001 1 Nopes = 1
b0010 2 Nopes = 2
b0011 3 Nopes = 1, 2
b0100 4 Nopes = 3
b0101 5 Nopes = 1, 3
b0110 6 Nopes = 2, 3
b0111 7 Skyscraper = 4
b1000 8 Nopes = 4
b1001 9 Nopes = 1, 4
b1010 10 Nopes = 2, 4
b1011 11 Skyscraper = 3
b1100 12 Nopes = 3, 4
b1101 13 Skyscraper
b1110 14 Skyscraper = 1
b1111 15 Invalid all nope
*/

class Field {
public:
    Field() = default;

    void insertSkyscraper(int skyscraper, std::size_t size);
    void insertNope(int nope);
    void insertNopes(const std::vector<int> &nopes);

    int skyscraper(std::size_t size) const;
    std::vector<int> nopes(std::size_t size) const;

    bool hasSkyscraper(std::size_t size) const;

    bool containsNope(int value) const;
    bool containsNopes(const std::vector<int> &values);

    BitmaskType bitmask() const;
    void setBitmask(BitmaskType bitmask);

private:
    bool bitIsToggled(BitmaskType bitmask, int bit) const;

    BitmaskType mBitmask{0};

    friend inline bool operator==(const Field &lhs, const Field &rhs);
    friend inline bool operator!=(const Field &lhs, const Field &rhs);
};

inline bool operator==(const Field &lhs, const Field &rhs)
{
    return lhs.mBitmask == rhs.mBitmask;
}
inline bool operator!=(const Field &lhs, const Field &rhs)
{
    return !(lhs == rhs);
}

#endif
